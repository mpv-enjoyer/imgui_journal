#pragma once
#include "attendance_wday.h"
#include "ids.h"

class Attendance_Wdays
{
    Vector_Sortable<Attendance_Wday> m_wdays;
public:
    Attendance_Wdays()
    {
        for (std::size_t i = 0; i < Wday::COUNT; i++)
        {
            m_wdays.push_back(Ptr<Attendance_Wday>::make());
        }
    }
    const Attendance_Wday& cref_wday(Wday wday) const
    {
        return *(m_wdays.cref_data().at(wday.get_EN()));
    }
    Attendance_Wday& ref_wday(Wday wday)
    {
        return *(m_wdays.ref_data().at(wday.get_EN()));
    }
    const Attendance_Merged_Lesson& cref_merged_lesson(Merged_Lesson_ID id) const
    {
        return cref_wday(id.wday()).cref_merged_lessons().cref(id.pos());
    }
    Attendance_Merged_Lesson& ref_merged_lesson(Merged_Lesson_ID id)
    {
        return ref_wday(id.wday()).ref_merged_lessons().ref(id.pos());
    }
    const Attendance_Internal_Lesson& cref_internal_lesson(Internal_Lesson_ID id) const
    {
        return cref_merged_lesson(id.merged_lesson_id()).cref_internal_lessons().cref(id.pos());
    }
    Attendance_Internal_Lesson& ref_internal_lesson(Internal_Lesson_ID id)
    {
        return ref_merged_lesson(id.merged_lesson_id()).ref_internal_lessons().ref(id.pos());
    }
    const Attendance_Student& cref_attendance_student(Internal_Student_ID id) const
    {
        return cref_internal_lesson(id.internal_lesson_id()).cref_student(id.pos());
    }
    Attendance_Student& ref_attendance_student(Internal_Student_ID id)
    {
        return ref_internal_lesson(id.internal_lesson_id()).ref_student(id.pos());
    }
    const Attendance_Holder& cref_attendance_holder(Attendance_ID id) const
    {
        return cref_attendance_student(id.internal_student_id()).cref_holder(id.aday());
    }
    Attendance_Holder& ref_attendance_holder(Attendance_ID id)
    {
        return ref_attendance_student(id.internal_student_id()).ref_holder(id.aday());
    }
};