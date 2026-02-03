#pragma once
#include "remove_student_from_lesson.h"
#include "add_student_to_lesson.h"

class Move_Student : public ICommand
{
    Merged_Lesson_ID m_from_merged_lesson_id;
    Pos<AStudent> m_from_student_pos;
    Merged_Lesson_ID m_to_merged_lesson_id;
    Pos<Student> get_student_pos(const IModel& model) const
    {
        return model->cref_merged_lesson(m_from_merged_lesson_id).get_student_positions()[m_from_student_pos.get()];
    }
public:
    Move_Student(Merged_Lesson_ID from_merged_lesson_id, Pos<AStudent> from_student_pos, Merged_Lesson_ID to_merged_lesson_id)
    : m_from_merged_lesson_id(from_merged_lesson_id), m_from_student_pos(from_student_pos), m_to_merged_lesson_id(to_merged_lesson_id) 
    { }
    Error get_error(const IModel& model) const override
    {
        Error error;
        false
         || (error = Remove_Student_From_Lesson(m_from_merged_lesson_id, m_from_student_pos).get_error(model))
         || (error = Add_Student_To_Lesson(m_to_merged_lesson_id, get_student_pos(model)).get_error(model));
        return error;
    }
    void call(IModel& model) override
    {
        Remove_Student_From_Lesson(m_from_merged_lesson_id, m_from_student_pos).call(model);
        Add_Student_To_Lesson(m_to_merged_lesson_id, get_student_pos(model)).call(model);
    }
};