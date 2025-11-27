#pragma once
#include "popup.h"

namespace View
{
    class Confirm_Delete_Lesson : public Popup
    {
        Merged_Lesson_ID m_id;

        bool render_logic() override
        {
            const Attendance_Merged_Lesson& merged_lesson = model()->cref_merged_lesson(m_id);
            UI::label("Номер группы: " + std::to_string(merged_lesson.get_number()));

            for (auto it = merged_lesson.cref_internal_lessons().cbegin(); !!it; ++it)
            {
                std::string lesson_type = Lesson_Infos::get_name(it->get_lesson_type());
                UI::label(std::to_string(it.get_position().get() + 1) + ". " + lesson_type);
                UI::label(it->get_time_begin().to_string() + " - " + it->get_time_end().to_string());
            }
            UI::label("Описание: " + merged_lesson.get_comment());
            UI::label("Возраст: " + merged_lesson.get_age_group_string());
            return true;
        }
        ICommand::Error call(IController& controller) override
        {
            auto action = Ptr<Remove_Lesson>::make(m_id);
            if (auto error = controller.get_error(action))
            {
                
            }
        }
        Ptr<ICommand> get_action() const override
        {
            return Ptr<Remove_Lesson>::make(m_id);
        }
        std::optional<std::string> get_error() const
        {
            return {};
        }
    public:
        Confirm_Delete_Lesson(IController& controller, Merged_Lesson_ID id)
        : Popup("Удалить группу?", controller), m_id(id)
        { }
    };
}