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
    const Removal_Info& cref_removal_info() const
    {
        return m_removal_info;
    }
    Removal_Info& ref_removal_info()
    {
        return m_removal_info;
    }
    const Vector<Attendance_Internal_Lesson>& cref_internal_lessons() const
    {
        return m_internal_lessons;
    }
    Vector<Attendance_Internal_Lesson>& ref_internal_lessons()
    {
        return m_internal_lessons;
    }
    void add_student(Attendance_Student* attendance_student)
    {
        auto iter = m_internal_lessons.begin();
        do
        {
            iter.get()->add_student(attendance_student);
        } while (iter.next());
        // ALL checks must be in Controller so omit even that one
        //for (const auto& attendance_students : m_attendance_students.data())
        //{
        //    if (student_pos.get() == attendance_students->get_student_pos().get())
        //    {
        //        return;
        //    }
        //}
    }
};