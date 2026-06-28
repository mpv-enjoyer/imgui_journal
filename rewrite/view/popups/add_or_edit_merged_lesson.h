#pragma once
#include "popup.h"

namespace View
{
    class Add_Or_Edit_Merged_Lesson : public Popup
    {
        const Wday m_wday;
        const std::optional<Merged_Lesson_ID> m_id;
        static UI::Combobox_Choices<Wday> get_wdays()
        {
            UI::Combobox_Choices<Wday> output;
            for (int i = 0; i < Wday::COUNT; i++)
            {
                auto wday = Wday::make_from_RU(i);
                output.emplace_back(wday.get_name_short(), wday);
            }
            return output;
        }
        static UI::Combobox_Choices<int> get_age_groups()
        {
            UI::Combobox_Choices<int> output;
            auto age_groups = Attendance_Merged_Lesson::get_all_age_groups();
            for (int i = 0; i < age_groups.size(); i++)
            {
                output.emplace_back(age_groups[i], i);
            }
            return output;
        }
        UI::Combobox<Wday> m_select_wday;
        UI::Select_Lesson_Type m_select_lesson_type;
        UI::Input_Int m_input_number;
        UI::Combobox<int> m_select_age_group;
        UI::Input_Text m_comment;
        struct Request
        {
            UI::Input_JTime begin;
            UI::Input_JTime end;
            Lesson_Type lesson_type;
            ::Add_Or_Edit_Merged_Lesson::Request convert() const
            {
                return ::Add_Or_Edit_Merged_Lesson::Request
                {
                    .type = lesson_type,
                    .begin = begin.get_value(),
                    .end = end.get_value()
                };
            }
            Request(int i, Lesson_Type type)
            : begin(std::to_string(i)), end(std::to_string(i)), lesson_type(type)
            { }
            Request(int i, const Attendance_Internal_Lesson& internal_lesson)
            : begin(std::to_string(i), internal_lesson.get_time_begin()), 
              end(std::to_string(i), internal_lesson.get_time_end()),
              lesson_type(internal_lesson.get_lesson_type())
            { }
        };
        std::vector<Request> m_requests;
        bool render_logic() override
        {
            {
                UI::Scope_Disabled disabled(m_id.has_value());
                m_select_wday.render();
                m_select_lesson_type.render();
            }

            for (size_t i = 0; i < m_requests.size(); i++)
            {
                if (m_requests.size() > 1)
                {
                    UI::label(std::to_string(i + 1) + ". " + Lesson_Infos::get_name(m_requests[i].lesson_type));
                }
                m_requests[i].begin.render();
                ImGui::SameLine();
                ImGui::Text(" _ ");
                ImGui::SameLine();
                m_requests[i].end.render();
            }
            m_input_number.render();
            m_select_age_group.render();
            m_comment.render();
            return true;
        }
        std::vector<std::shared_ptr<ICommand>> get_actions() const
        {
            std::vector<::Add_Or_Edit_Merged_Lesson::Request> requests;
            for (auto request : m_requests)
            {
                requests.push_back(request.convert());
            }

            if (m_id)
            {
                return { std::make_shared<::Add_Or_Edit_Merged_Lesson>(*m_id, m_input_number.get_value(), m_comment.get_value(), m_select_age_group.get_choice(), requests) };
            }
            else
            {
                return { std::make_shared<::Add_Or_Edit_Merged_Lesson>(Month::make_current(), m_select_wday.get_choice(), m_input_number.get_value(), m_comment.get_value(), m_select_age_group.get_choice(), requests) };
            }
        }
        std::optional<std::string> get_error() const
        {
            return {};
        }
    public:
        Add_Or_Edit_Merged_Lesson(IController& controller, Merged_Lesson_ID id)
        : Popup("Изменить группу", controller), m_wday(id.wday()), m_id(id),
        m_select_wday("День недели", get_wdays(), id.wday().get_RU()),
        m_select_lesson_type("Программа", nullptr, &(model()->cref_merged_lesson(id))),
        m_input_number("Номер", model()->cref_merged_lesson(id).get_number()),
        m_select_age_group("Возраст", get_age_groups(), model()->cref_merged_lesson(id).get_age_group()),
        m_comment("Описание (необязательно)", model()->cref_merged_lesson(id).get_comment())
        {
            const auto& internal_lessons = model()->cref_merged_lesson(id).cref_internal_lessons();
            for (int i = 0; i < internal_lessons.size(); i++)
            {
                auto pos = Position<Attendance_Internal_Lesson>(i);
                m_requests.emplace_back(i, internal_lessons[pos]);
            }
        }
        Add_Or_Edit_Merged_Lesson(IController& controller, Wday wday)
        : Popup("Добавить группу", controller), m_wday(wday),
        m_select_wday("День недели", get_wdays(), wday.get_RU()),
        m_select_lesson_type("Программа", [&](std::vector<Lesson_Type> lesson_types)
        {
            JTime time_default(0, 0);
            std::vector<Request> requests = m_requests;
            for (int i = m_requests.size(); i < lesson_types.size(); i++)
            {
                m_requests.emplace_back(i, lesson_types[i]);
            }
            for ( ; lesson_types.size() < m_requests.size(); )
            {
                m_requests.erase(--(m_requests.end()));
            }
            return true;
        }),
        m_input_number("Номер"),
        m_select_age_group("Возраст", get_age_groups()),
        m_comment("Описание (необязательно)", "")
        {
            m_select_lesson_type.trigger_callback();
        }
    };
}