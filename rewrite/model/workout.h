#pragma once
#include "ids.h"

class AWdays;

class Workout
{
    AWdays* m_source;
    Attendance_ID m_should_id;
    Internal_Lesson_ID m_real_internal_lesson_id;
    Aday m_real_aday;
public:
    Workout(AWdays& source, Attendance_ID should_id, Internal_Lesson_ID real_internal_lesson_id, Aday real_aday)
    : m_source(&source), m_should_id(should_id), m_real_internal_lesson_id(real_internal_lesson_id), m_real_aday(real_aday) { }
    Attendance_ID should_id() const { return m_should_id; }
    Internal_Lesson_ID real_internal_lesson_id() const { return m_real_internal_lesson_id; }
    Aday real_aday() const { return m_real_aday; }
    AUTOEQ3(Workout, m_should_id, m_real_internal_lesson_id, m_real_aday);
};
