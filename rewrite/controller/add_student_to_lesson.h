#pragma once
#include "icommand.h"

class Add_Student_To_Lesson : public ICommand
{
    Merged_Lesson_ID m_merged_lesson_id;
    Position<Student> m_student_pos;
public:
    Add_Student_To_Lesson(Merged_Lesson_ID merged_lesson_id, Position<Student> student_pos)
    : m_merged_lesson_id(merged_lesson_id), m_student_pos(student_pos)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        if (model->students()->cref_students().cref(m_student_pos).is_removed())
        {
            return "cannot add removed student";
        }
        auto& merged_lesson = model->attendance_wdays()->cref_merged_lesson(m_merged_lesson_id);
        if (merged_lesson.is_removed()) return "cannot add to removed lesson";
        auto internal_students = merged_lesson.get_student_positions();
        bool exists = std::any_of(internal_students.begin(), internal_students.end(), [=](const Position<Student>& student)
            {
                return student == m_student_pos;
            });
        if (exists) return "Ученик уже есть в группе";
        return {};
    }
    void call(IModel& model) override
    {
        std::size_t count = m_merged_lesson_id.wday().calculate_count_for_bottom_year(model->bottom_year);
        model->attendance_wdays()->ref_merged_lesson(m_merged_lesson_id).add_student(count, m_student_pos);
    }
};