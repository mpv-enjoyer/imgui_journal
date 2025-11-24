#pragma once
#include "icommand.h"

class Edit_Wants_Lesson : public ICommand
{
    Internal_Student_ID m_id;
    bool m_enable;
public:
    Edit_Wants_Lesson(Internal_Student_ID id, bool enable)
    : m_id(id), m_enable(enable)
    { }
    Error get_error(const IModel& model) const override
    {
        const auto& attendance_student = model->attendance_wdays()->cref_attendance_student(m_id);
        auto student_pos = attendance_student.get_student_pos();
        if (model->students()->cref_students()[student_pos].is_removed())
        {
            return "Ученик удален";
        }
        if (model->attendance_wdays()->cref_merged_lesson(m_id).is_removed())
        {
            return "Группа удалена";
        }
        if (model->attendance_wdays()->cref_merged_lesson(m_id).is_student_removed(m_id.pos()))
        {
            return "Ученик удален из группы";
        }
        return {};
    }
    void call(IModel& model)
    {
        model->attendance_wdays()->ref_attendance_student(m_id).set_wants_lesson(m_enable);
    }
};