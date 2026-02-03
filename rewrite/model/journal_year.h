#pragma once
#include "common/ptr.h"
#include "workouts.h"
#include "holidays.h"
#include "attendance_wdays.h"
#include "teachers.h"
#include "students.h"
#include "lesson_infos.h"

class Journal_Year
{
    Ptr<Workouts> m_workouts = Ptr<Workouts>::make();
    Ptr<Holidays> m_holidays = Ptr<Holidays>::make();
    Ptr<AWdays> m_attendance_wdays = Ptr<AWdays>::make();
    Ptr<Teachers> m_teachers = Ptr<Teachers>::make();
    Ptr<Students> m_students = Ptr<Students>::make();
    Ptr<Lesson_Infos> m_lesson_infos = Ptr<Lesson_Infos>::make();
public:
    const Year bottom_year;
    explicit Journal_Year(Year year) : bottom_year(year) { }
    const Ptr<Workouts>& workouts() const { return m_workouts; }
          Ptr<Workouts>& workouts()       { return m_workouts; }
    const Ptr<Holidays>& holidays() const { return m_holidays; }
          Ptr<Holidays>& holidays()       { return m_holidays; }
    const Ptr<AWdays>& attendance_wdays() const { return m_attendance_wdays; }
          Ptr<AWdays>& attendance_wdays()       { return m_attendance_wdays; }
    const Ptr<Teachers>& teachers() const { return m_teachers; }
          Ptr<Teachers>& teachers()       { return m_teachers; }
    const Ptr<Students>& students() const { return m_students; }
          Ptr<Students>& students()       { return m_students; }
    const Ptr<Lesson_Infos>& lesson_infos() const { return m_lesson_infos; }
          Ptr<Lesson_Infos>& lesson_infos()       { return m_lesson_infos; }
    
    const AWday& cref_wday(Wday wday) const
    {
        return attendance_wdays()->cref_wday(wday);
    }
    AWday& ref_wday(Wday wday)
    {
        return attendance_wdays()->ref_wday(wday);
    }
    const AMerged_Lesson& cref_merged_lesson(Merged_Lesson_ID id) const
    {
        return cref_wday(id.wday()).cref_merged_lessons().cref(id.pos());
    }
    AMerged_Lesson& ref_merged_lesson(Merged_Lesson_ID id)
    {
        return ref_wday(id.wday()).ref_merged_lessons().ref(id.pos());
    }
    const AInternal_Lesson& cref_internal_lesson(Internal_Lesson_ID id) const
    {
        return cref_merged_lesson(id.merged_lesson_id()).cref_internal_lessons().cref(id.pos());
    }
    AInternal_Lesson& ref_internal_lesson(Internal_Lesson_ID id)
    {
        return ref_merged_lesson(id.merged_lesson_id()).ref_internal_lessons().ref(id.pos());
    }
    const AStudent& cref_attendance_student(Internal_Student_ID id) const
    {
        return cref_internal_lesson(id.internal_lesson_id()).cref_student(id.pos());
    }
    AStudent& ref_attendance_student(Internal_Student_ID id)
    {
        return ref_internal_lesson(id.internal_lesson_id()).ref_student(id.pos());
    }
    const AHolder& cref_attendance_holder(Attendance_ID id) const
    {
        return cref_attendance_student(id.internal_student_id()).cref_holder(id.aday());
    }
    AHolder& ref_attendance_holder(Attendance_ID id)
    {
        return ref_attendance_student(id.internal_student_id()).ref_holder(id.aday());
    }
};