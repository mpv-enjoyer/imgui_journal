#pragma once
#include "icommand.h"

// UNUSED - scrapped idea - just use edit_wants_lesson

/*
class Set_Student_Attend_Begin : public ICommand
{
    Merged_Lesson_ID m_merged_lesson_id;
    Pos<AStudent> m_student_pos;
    Month m_month;
public:
    Set_Student_Attend_Begin(Merged_Lesson_ID merged_lesson_id, Pos<AStudent> student_pos, Month month)
    : m_merged_lesson_id(merged_lesson_id), m_student_pos(student_pos), m_month(month) { }
    Error get_error(const IModel& model) const override
    {
        const AMerged_Lesson& merged_lesson = model->cref_merged_lesson(m_merged_lesson_id);
        std::optional<Month> old = merged_lesson.get_student_attend_begin(m_student_pos);// m_students_info[student_pos.get()].begin;
        if (old)
        {
        TODO_CRITICAL(this);
            if (m_month < (*old))
            {
                Aday old_aday = Aday::make_from_first_wday(m_merged_lesson_id.wday(), *old);
                Aday new_aday = Aday::make_from_first_wday(m_merged_lesson_id.wday(), m_month);
                DEBUG_ASSERT(new_aday.index() < old_aday.index());
                for (Aday current = new_aday; current.index() < old_aday.index(); current.next())
                {
                    for (const auto& internal_lesson : merged_lesson.cref_internal_lessons())
                    {
                        AStatus status = internal_lesson.cref_student(m_student_pos).cref_holder(current).get_status();
                        if (status == AStatus::ON_LESSON)
                        {
                            return "";
                        }
                    }
                }
            }
        }
        if (model->holidays()->get_holiday(m_mday)) return "На этот день уже назначен праздник";
        return {};
    }
    void call(IModel& model) override
    {
        Holiday holiday(m_mday);
        holiday.set_reason(m_reason);
        model->holidays()->add_holiday(holiday);
    }
};
*/