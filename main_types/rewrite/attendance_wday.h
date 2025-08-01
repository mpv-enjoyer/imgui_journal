#pragma once
#include "attendance_merged_lesson.h"

class Attendance_Wday
{
    Vector<Attendance_Merged_Lesson> m_merged_lessons;
public:
    Attendance_Wday()
    { }
    const Vector<Attendance_Merged_Lesson>& cref_merged_lessons() const
    {
        return m_merged_lessons;
    }
    AUTOMUT0(ref_merged_lessons)
};