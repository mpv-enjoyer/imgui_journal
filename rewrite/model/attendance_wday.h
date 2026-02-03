#pragma once
#include "attendance_merged_lesson.h"

class AWday
{
    Vector_Sortable<AMerged_Lesson> m_merged_lessons;
public:
    AWday()
    { }
    const Vector_Sortable<AMerged_Lesson>& cref_merged_lessons() const
    {
        return m_merged_lessons;
    }
    Vector_Sortable<AMerged_Lesson>& ref_merged_lessons()
    {
        return m_merged_lessons;
    }
};