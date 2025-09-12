#pragma once
#include "attendance_internal_lesson.h"

class Attendance_Merged_Lesson
{
    Removal_Info m_removal_info;
    Vector_Sortable<Attendance_Internal_Lesson> m_internal_lessons;
public:
    Attendance_Merged_Lesson(std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons)
    {
        IM_ASSERT(internal_lessons.size() != 0);
        for (auto& internal_lesson : internal_lessons)
        {
            m_internal_lessons.push_back(std::move(internal_lesson));
        }
    }
    const Removal_Info& cref_removal_info() const
    {
        return m_removal_info;
    }
    Removal_Info& ref_removal_info()
    {
        return m_removal_info;
    }
    const Vector_Sortable<Attendance_Internal_Lesson>& cref_internal_lessons() const
    {
        return m_internal_lessons;
    }
    Vector_Sortable<Attendance_Internal_Lesson>& ref_internal_lessons()
    {
        return m_internal_lessons;
    }
    void add_student(Year bottom_year, Wday wday, Vector_Sortable<Student>::Position student_pos)
    {
        for (auto iter = m_internal_lessons.begin(); iter; iter.next())
        {
            iter->add_student(bottom_year, wday, student_pos);
        }
        // Kind of weird that we pass bottom_year here but that's
        // the cost of not having const Model& anywhere I guess?
    }
    const Vector_Sortable<Attendance_Student>& cref_students() const
    {
        return m_internal_lessons.cbegin()->cref_data();
    }
    Vector_Sortable<Attendance_Student>& ref_students()
    {
        return m_internal_lessons.begin()->ref_data();
    }
};