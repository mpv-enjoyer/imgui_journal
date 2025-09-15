#pragma once
#include "icommand.h"

class Add_Student_To_Group : public ICommand
{
    const Wday m_wday;
    const Vector_Sortable<Attendance_Merged_Lesson>::Position m_merged_lesson_pos;
    const Vector_Sortable<Student>::Position m_student_pos;
public:
    Add_Student_To_Group(Wday wday, Vector_Sortable<Attendance_Merged_Lesson>::Position merged_lesson_pos, Vector_Sortable<Student>::Position student_pos)
    : m_wday(wday), m_merged_lesson_pos(merged_lesson_pos), m_student_pos(student_pos)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        const auto& students = model->students()->cref_students();
        if (!students.is_pos_valid(m_student_pos)) return "m_student_pos invalid";
        const auto& merged = model->attendance_wdays()->cref_wday(m_wday).cref_merged_lessons();
        if (!merged.is_pos_valid(m_merged_lesson_pos)) return "m_merged_lesson_pos invalid";
        const auto& internal_students_data = merged.cref(m_merged_lesson_pos).cref_students().cref_data();
        bool exists = std::any_of(internal_students_data.begin(), internal_students_data.end(), [=](const Attendance_Student& student)
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