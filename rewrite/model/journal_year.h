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
    Ptr<Attendance_Wdays> m_attendance_wdays = Ptr<Attendance_Wdays>::make();
    Ptr<Teachers> m_teachers = Ptr<Teachers>::make();
    Ptr<Students> m_students = Ptr<Students>::make();
    Ptr<Lesson_Infos> m_lesson_infos = Ptr<Lesson_Infos>::make();
    const Year m_bottom_year;
public:
    explicit Journal_Year(Year bottom_year) : m_bottom_year(bottom_year) { }
    Mday get_mday(Wday wday, Aday aday) const { return Mday::make_from_aday(m_bottom_year, wday, aday); }
    size_t get_aday_count(Wday wday) const { return wday.calculate_count_for_bottom_year(m_bottom_year); }
    Month get_month_begin() const { return Month::make_begin_study_year_from_bottom_year(m_bottom_year); }
    bool is_within_current(Month month) const { return month.get_study_bottom_year() == m_bottom_year; }

    const Ptr<Workouts>& workouts() const { return m_workouts; }
          Ptr<Workouts>& workouts()       { return m_workouts; }
    const Ptr<Holidays>& holidays() const { return m_holidays; }
          Ptr<Holidays>& holidays()       { return m_holidays; }
    const Ptr<Attendance_Wdays>& attendance_wdays() const { return m_attendance_wdays; }
          Ptr<Attendance_Wdays>& attendance_wdays()       { return m_attendance_wdays; }
    const Ptr<Teachers>& teachers() const { return m_teachers; }
          Ptr<Teachers>& teachers()       { return m_teachers; }
    const Ptr<Students>& students() const { return m_students; }
          Ptr<Students>& students()       { return m_students; }
    const Ptr<Lesson_Infos>& lesson_infos() const { return m_lesson_infos; }
          Ptr<Lesson_Infos>& lesson_infos()       { return m_lesson_infos; }
    
    const Attendance_Wday& cref_wday(Wday wday) const
    {
        return attendance_wdays()->cref_wday(wday);
    }
    Attendance_Wday& ref_wday(Wday wday)
    {
        return attendance_wdays()->ref_wday(wday);
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