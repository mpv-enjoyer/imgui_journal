#pragma once
#include "attendance_internal_lesson.h"

class Attendance_Merged_Lesson
{
    Removal_Info m_removal_info;
    Vector<Attendance_Internal_Lesson> m_internal_lessons;
public:
    Attendance_Merged_Lesson(std::vector<Attendance_Internal_Lesson*> internal_lessons)
    {
        for (auto internal_lesson : internal_lessons)
        {
            m_internal_lessons.push_back(internal_lesson);
        }
    }
    Removal_Info& ref_removal_info()
    {
        return m_removal_info;
    }
    const Removal_Info& сref_removal_info() const
    {
        return m_removal_info;
    }
};