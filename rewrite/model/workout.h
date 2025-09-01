#pragma once
#include "attendance_merged_lesson.h"
#include "student.h"

class Workout
{
public:
    struct Position
    {
        Mday mday;
        Vector_Sortable<Attendance_Merged_Lesson>::Position merged_lesson_pos;
        Vector_Sortable<Attendance_Internal_Lesson>::Position internal_lesson_pos;
        bool operator==(const Position& other) const
        {
            return std::tie(this->mday, this->merged_lesson_pos, this->internal_lesson_pos)
                == std::tie(other.mday, other.merged_lesson_pos, other.internal_lesson_pos);
        }
    };
    Workout(Position real_pos, Position should_pos, Vector_Sortable<Student>::Position student_pos)
    : m_real_pos(real_pos), m_should_pos(should_pos), m_student_pos(student_pos)
    { }
    Position get_real_pos() const
    {
        return m_real_pos;
    }
    Position get_should_pos() const
    {
        return m_should_pos;
    }
    Vector_Sortable<Student>::Position get_student_pos() const
    {
        return m_student_pos;
    }
    bool operator==(const Workout& other) const
    {
        return std::tie(this->m_real_pos, this->m_should_pos, this->m_student_pos)
            == std::tie(other.m_real_pos, other.m_should_pos, other.m_student_pos);
    }
private:
    Position m_real_pos;
    Position m_should_pos;
    Vector_Sortable<Student>::Position m_student_pos;
};