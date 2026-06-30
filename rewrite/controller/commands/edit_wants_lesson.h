#pragma once
#include "icommand.h"

class Edit_Wants_Lesson : public ICommand
{
    Month m_month;
    Internal_Student_ID m_id;
    bool m_enable;
public:
    Edit_Wants_Lesson(Month month, Internal_Student_ID id, bool enable)
    : m_month(month), m_id(id), m_enable(enable)
    { }

    CMD_WANT_STATE(Time_State::CurrentYear)

    Error get_error(const IModel& model) const override
    {
        const auto& attendance_student = model->cref_attendance_student(m_id);
        auto student_pos = attendance_student.get_student_pos();
        if (model->students()->cref_students()[student_pos].is_removed(m_month))
        {
            return "Ученик удален";
        }
        if (model->cref_merged_lesson(m_id).is_removed(m_month))
        {
            return "Группа удалена";
        }
        if (model->cref_merged_lesson(m_id).is_student_removed(m_id.pos(), m_month))
        {
            return "Ученик удален из группы";
        }
        return {};
    }
    void call(IModel& model)
    {
        model->ref_attendance_student(m_id).set_wants_lesson(m_enable);
    }
};