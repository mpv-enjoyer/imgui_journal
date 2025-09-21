#pragma once
#include "icommand.h"

class Add_Student_To_Lesson : public ICommand
{
    const Wday m_wday;
    const Position<Attendance_Merged_Lesson> m_merged_lesson_pos;
    const Position<Student> m_student_pos;
public:
    Add_Student_To_Lesson(Wday wday, Position<Attendance_Merged_Lesson> merged_lesson_pos, Position<Student> student_pos)
    : m_wday(wday), m_merged_lesson_pos(merged_lesson_pos), m_student_pos(student_pos)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        const auto& students = model->students()->cref_students();
        if (!students.is_pos_valid(m_student_pos)) return "m_student_pos invalid";
        if (students.cref(m_student_pos).cref_removal_info().is_removed()) return "cannot add removed student";
        const auto& merged_lessons = model->attendance_wdays()->cref_wday(m_wday).cref_merged_lessons();
        if (!merged_lessons.is_pos_valid(m_merged_lesson_pos)) return "m_merged_lesson_pos invalid";
        if (merged_lessons.cref(m_merged_lesson_pos).cref_removal_info().is_removed()) return "cannot add to removed lesson";
        const auto& internal_students = merged_lessons.cref(m_merged_lesson_pos).cref_students();
        bool exists = std::any_of(internal_students.begin(), internal_students.end(), [=](const Attendance_Student& student)
            {
                return student.get_student_pos() == m_student_pos;
            });
        if (exists) return "m_student_pos already present in merged lesson";
        return {};
    }
    void call(IModel& model) override
    {
        model->attendance_wdays()
            ->ref_wday(m_wday)
            .ref_merged_lessons()
            .ref(m_merged_lesson_pos)
            .add_student(model->bottom_year, m_wday, m_student_pos);
    }
};