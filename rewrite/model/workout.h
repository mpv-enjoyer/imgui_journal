#pragma once
#include "attendance_merged_lesson.h"
#include "student.h"
#include "ids.h"

class Workout
{
public:
    Workout(Workout_Lesson_ID real_pos, Workout_Lesson_ID should_pos, Vector_Sortable<Student>::Position student_pos)
    : m_real_pos(real_pos), m_should_pos(should_pos), m_student_pos(student_pos)
    { }
    Workout_Lesson_ID get_real_pos() const
    {
        return m_real_pos;
    }
    Workout_Lesson_ID get_should_pos() const
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
    Workout_Lesson_ID m_real_pos;
    Workout_Lesson_ID m_should_pos;
    Vector_Sortable<Student>::Position m_student_pos;
};