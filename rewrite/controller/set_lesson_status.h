#pragma once
#include "icommand.h"

// void set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed);

class Set_Lesson_Status : public ICommand
{
    Attendance_ID m_id;
public:
    Set_Lesson_Status(Attendance_ID id, Attendance_Status status)
    : m_id(id)
    { }
    Error get_error(const IModel& model) override
    {
        if (model->holidays()->get_holiday(m_id.aday().mday())) return "На этот день назначен выходной";
        return {};
    }
    void call(IModel& model) override
    {
        auto student_pos = model->attendance_wdays()->cref_attendance_student(m_id.internal_student_id()).get_student_pos();
        auto contract_pos = model->students()->cref_students().cref(student_pos).get_contract_pos();
        model->attendance_wdays()->ref_attendance_student(m_id.internal_student_id()).get_holder(m_id.aday());
        TODO_CRITICAL(I need to keep workouts in mind);
        model->attendance_wdays()->ref_merged_lesson(m_id).ref_removal_info().remove();
    }
};
