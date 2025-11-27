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
                if (m_requests.size() != 0)
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
        std::vector<Ptr<ICommand>> get_actions() const
        {
            std::vector<::Add_Or_Edit_Merged_Lesson::Request> requests;
            for (auto request : m_requests)
            {
                requests.push_back(request.convert());
            }

            if (m_id)
            {
                std::vector<bool> adays;
                for (auto aday : model()->cref_merged_lesson(*m_id).get_adays())
                {
                    adays.push_back(aday.is_active);
                    // TODO: give user power to edit this
                }
                return { Ptr<::Add_Or_Edit_Merged_Lesson>::make(*m_id, adays, m_input_number.get_value(), m_comment.get_value(), m_select_age_group.get_choice(), requests) };
            }
            else
            {
                // TODO: adding only current day here rn!!! GET MDAY!!!
                return { Ptr<::Add_Or_Edit_Merged_Lesson>::make(Mday::make_current(), m_input_number.get_value(), m_comment.get_value(), m_select_age_group.get_choice(), requests) };
            }
        }
        std::optional<std::string> get_error() const
        {
            return {};
        }
    public:
        Add_Or_Edit_Merged_Lesson(IController& controller, Merged_Lesson_ID id)
        : Popup("Изменить группу " + id.wday().get_name(), controller), m_wday(id.wday()), m_id(id),
        m_select_wday("День недели", get_wdays(), id.wday().get_RU()),
        m_select_lesson_type("Программа", nullptr, &(model()->cref_merged_lesson(id))),
        m_input_number("Номер", model()->cref_merged_lesson(id).get_number()),
        m_select_age_group("Возраст", get_age_groups(), model()->cref_merged_lesson(id).get_age_group()),
        m_comment("Описание (необязательно)", model()->cref_merged_lesson(id).get_comment())
        { }
        Add_Or_Edit_Merged_Lesson(IController& controller, Wday wday)
        : Popup("Добавить группу на " + wday.get_name(), controller), m_wday(wday),
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
        { }
    };
}