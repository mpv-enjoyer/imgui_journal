#pragma once
#include "view/ui/ui.h"
#include "model/imodel.h"

namespace UI
{
    class Select_Students : public AUnit
    {
        struct Student_Data
        {
            Position<Student> position;
            bool enabled;
            std::string description;
            Student_Data(Position<Student> position_, std::string description_)
            : position(position_), enabled(false), description(description_) { }
        };
        std::vector<Student_Data> m_data;
        bool m_multi;
        ImGuiTextFilter m_filter;
    public:
        Select_Students(std::string id, const IModel& model, std::function<bool(Position<Student> student)> filter, bool multi)
        : AUnit(id), m_multi(multi)
        {
            for (auto it = model->get_all_students_sorted(); it; ++it)
            {
                if (!filter || filter(it.get_position()))
                {
                    int contract_number = model->students()->cref_contracts()[it->get_contract_pos()].get_number();
                    m_data.emplace_back(it.get_position(), it->get_name() + " (" + std::to_string(contract_number) + ")");
                }
            }
        }
        void render_logic()
        {
            Scope_Group group;
            {
                Scope_Color_Input color;
                m_filter.Draw("Поиск с учётом регистра");
            }
            Scope_Child child("Child window", ImVec2(500, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
            for (auto& student_data : m_data)
            {
                if (!m_filter.PassFilter(student_data.description.c_str())) continue;
                bool enabled_before = student_data.enabled;
                std::string id = (enabled_before ? "Выбран.##" : "Выбрать##") + std::to_string(student_data.position.get());
                if (button_selectable(id, enabled_before))
                {
                    if (m_multi) student_data.enabled = !enabled_before;
                    if (!m_multi && !enabled_before)
                    {
                        auto it = std::find_if(m_data.begin(), m_data.end(), [](const Student_Data& student_data) {
                            return student_data.enabled;
                        });
                        if (it != m_data.end()) it->enabled = false;
                        student_data.enabled = true;
                    }
                }
                ImGui::SameLine();
                label(student_data.description);
            }
        }
        std::optional<Position<Student>> get_student_position() const
        {
            if (m_multi) return {};
            auto it = std::find_if(m_data.begin(), m_data.end(), [](const Student_Data& student_data) { return student_data.enabled; });
            if (it == m_data.end()) return {};
            else return it->position;
        }
        std::vector<Position<Student>> get_student_positions() const
        {
            if (!m_multi) return {};
            std::vector<Position<Student>> result;
            result.reserve(m_data.size());
            for (const auto& student_data : m_data)
            {
                if (student_data.enabled) result.push_back(student_data.position);
            }
            return result;
        }
    };
}