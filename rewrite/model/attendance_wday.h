#pragma once
#include "attendance_merged_lesson.h"

class Attendance_Wday
{
    Vector_Sortable<Attendance_Merged_Lesson> m_merged_lessons;
public:
    Attendance_Wday()
    { }
    const Vector_Sortable<Attendance_Merged_Lesson>& cref_merged_lessons() const
    {
        return m_merged_lessons;
    }
    Vector_Sortable<Attendance_Merged_Lesson>& ref_merged_lessons()
    {
        return m_merged_lessons;
    }
};