#pragma once
#include "icommand.h"

class Edit_Wants_Lesson : public ICommand
{
    Internal_Student_ID m_id;
    bool m_enable;
    Month m_month;
    bool m_change_all_future_months;
public:
    Edit_Wants_Lesson(Internal_Student_ID id, bool enable, Month month, bool change_all_future_months)
    : m_id(id), m_enable(enable), m_month(month), m_change_all_future_months(change_all_future_months)
    { }
    Error get_error(const IModel& model) const override
    {
        const auto& attendance_student = model->cref_attendance_student(m_id);
        auto student_pos = attendance_student.get_student_pos();
        if (model->students()->cref_students()[student_pos].is_removed())
        {
            return "Ученик удален";
        }
        if (model->cref_merged_lesson(m_id).is_removed())
        {
            return "Группа удалена";
        }
        if (model->cref_merged_lesson(m_id).is_student_removed(m_id.pos()))
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