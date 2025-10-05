#pragma once
#include "icommand.h"

// void set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed);

class Set_Lesson_Status : public ICommand
{
    Attendance_ID m_id;
    Attendance_Status m_status;
public:
    Set_Lesson_Status(Attendance_ID id, Attendance_Status status)
    : m_id(id), m_status(status)
    { }
    Error get_error(const IModel& model) override
    {
        if (model->holidays()->get_holiday(m_id.aday().mday())) return "На этот день назначен выходной";
        auto current_status = model->attendance_wdays()->cref_attendance_holder(m_id).get_status();
        if (current_status == Attendance_Status::NOT_AWAITED)
        {
            // Handle legacy NAW's:
            return "Ученик не должен приходить на этот урок";
        }
        return {};
    }
    void call(IModel& model) override
    {
        auto& attendance_student = model->attendance_wdays()->ref_attendance_student(m_id);
        auto& holder = attendance_student.ref_holder(m_id.aday());
        
        auto student_pos = attendance_student.get_student_pos();
        if (auto workout = model->workouts()->is_should(m_id))
        {
            model->workouts()->remove(*workout);
        }
        holder.set(m_status, Helpers::get_default_discount_id(*model, student_pos));
    }
};
