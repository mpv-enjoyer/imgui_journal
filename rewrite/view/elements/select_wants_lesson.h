#pragma once
#include "view/ui/ui.h"
#include "model/lesson_infos.h"
#include "model/attendance_merged_lesson.h"

namespace UI
{
    void select_wants_lesson(std::string id, const Attendance_Merged_Lesson& merged_lesson, Position<Attendance_Student> attendance_student_pos, std::function<bool(std::vector<bool>)> callback)
    {
        DEBUG_ASSERT(merged_lesson.cref_internal_lessons().size() > 0);
        Combobox_Choices<std::vector<bool>> choices;

        // Starting from 1 because at least one internal lesson is always 'wanted'
        for (size_t i = 1; i < (1 << merged_lesson.cref_internal_lessons().size()); i++)
        {
            std::stringstream s;
            std::vector<bool> current_choice;
            for (size_t internal_lesson_id = 0; internal_lesson_id < merged_lesson.cref_internal_lessons().size(); internal_lesson_id++)
            {
                bool enabled = i & (1 << internal_lesson_id);
                current_choice.push_back(enabled);
                if (internal_lesson_id != 0) s << "+";
                if (enabled)
                {
                    Position<Attendance_Internal_Lesson> internal_lesson_pos(internal_lesson_id);
                    s << Lesson_Infos::get_name(merged_lesson.cref_internal_lessons()[internal_lesson_pos].get_lesson_type());
                }
                else
                {
                    s << " _ ";
                }
            }
            choices.emplace_back(s, current_choice);
        }
        size_t merged_lesson_current_value = 0;
        for (size_t i = 0; i < merged_lesson.cref_internal_lessons().size(); i++)
        {
            Position<Attendance_Internal_Lesson> internal_lesson_pos(i);
            if (merged_lesson.cref_internal_lessons()[internal_lesson_pos].cref_student(attendance_student_pos).get_wants_lesson())
            {
                merged_lesson_current_value += 1 << i;
            }
        }
        DEBUG_ASSERT(merged_lesson_current_value != 0);
        merged_lesson_current_value--;
        Combobox<std::vector<bool>>(id, choices, merged_lesson_current_value, callback).render();
    }
} // namespace UI
