#pragma once
#include "icommand.h"

// void remove_student_from_group(int wday, int merged_lesson_id, int student_id);

class Remove_Student_From_Lesson : public ICommand
{
    Merged_Lesson_ID m_merged_lesson_id;
    Position<Attendance_Student> m_student_pos;
public:
    Remove_Student_From_Lesson(Merged_Lesson_ID merged_lesson_id, Position<Attendance_Student> student_pos)
    : m_merged_lesson_id(merged_lesson_id), m_student_pos(student_pos)
    { }
    std::optional<std::string> get_error(const IModel& model) const override
    {
        if (model->cref_merged_lesson(m_merged_lesson_id).is_student_removed(m_student_pos)) return "Ученик уже удален";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_merged_lesson_id).remove_student(m_student_pos);
    }
};

class Restore_Student_To_Lesson : public ICommand
{
    Merged_Lesson_ID m_merged_lesson_id;
    Position<Attendance_Student> m_student_pos;
public:
    Restore_Student_To_Lesson(Merged_Lesson_ID merged_lesson_id, Position<Attendance_Student> student_pos)
    : m_merged_lesson_id(merged_lesson_id), m_student_pos(student_pos)
    { }
    std::optional<std::string> get_error(const IModel& model) const override
    {
        if (!model->cref_merged_lesson(m_merged_lesson_id).is_student_removed(m_student_pos)) return "Ученик ещё не удален";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_merged_lesson_id).restore_student(m_student_pos);
    }
};