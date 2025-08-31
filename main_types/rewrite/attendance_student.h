#pragma once
#include "attendance_holder.h"
#include "removal_info.h"
#include "student.h"
#include "automut.h"

class Attendance_Student
{
    const Vector<Student>::Position m_student_pos;
    bool m_wants_lesson = true;
    Removal_Info m_removal_info;
    std::vector<std::vector<Ptr<Attendance_Holder>>> m_holders; // [month][mday]
    const Wday DEBUG_WDAY;
    const Year DEBUG_YEAR;
public:
    Attendance_Student(Year bottom_year, Wday wday, Vector<Student>::Position student_pos)
    : m_student_pos(student_pos), DEBUG_WDAY(wday), DEBUG_YEAR(bottom_year) 
    {
        m_holders.reserve(Month::COUNT);
        auto month = Month::make_begin_study_year(bottom_year);
        do
        {
            m_holders.emplace_back(month.calculate_wday_count(wday));
        } while (month.next());
    }
    const Removal_Info& cref_removal_info() const
    {
        return m_removal_info;
    }
    Removal_Info& ref_removal_info()
    {
        return m_removal_info;
    }
    bool get_wants_lesson() const
    {
        return m_wants_lesson;
    }
    void set_wants_lesson(bool value)
    {
        m_wants_lesson = value;
    }
    const Ptr<Attendance_Holder>& cref_holder(Mday mday) const // Mday must have the same Wday and Year as was passed eariler
    {
        DEBUG_ASSERT(Wday::make_from_mday(mday).get_EN() == DEBUG_WDAY.get_EN());
        DEBUG_ASSERT(mday.get_year().get_from_0() == DEBUG_YEAR.get_from_0());
        auto month_index = mday.get_month().calculate_study_year_index();
        auto mday_index = mday.get_index_in_month();
        return m_holders[month_index][mday_index];
    }
    Ptr<Attendance_Holder>& ref_holder(Mday mday) // Mday must have the same Wday and Year as was passed eariler
    {
        DEBUG_ASSERT(Wday::make_from_mday(mday).get_EN() == DEBUG_WDAY.get_EN());
        DEBUG_ASSERT(mday.get_year().get_from_0() == DEBUG_YEAR.get_from_0());
        auto month_index = mday.get_month().calculate_study_year_index();
        auto mday_index = mday.get_index_in_month();
        return m_holders[month_index][mday_index];
    }
    Attendance_Holder get_holder_unchecked(Mday mday) const // Mday must have the same Wday and Year as was passed eariler
    {
        return *cref_holder(mday);
    }
    Attendance_Holder get_holder(Mday mday) const // Mday must have the same Wday and Year as was passed eariler
    {
        auto value = get_holder_unchecked(mday);
        if (value.get_status() == Attendance_Status::NO_DATA)
        {
            if (!m_wants_lesson || m_removal_info.is_removed())
            {
                value.set(Attendance_Status::NOT_AWAITED, value.get_discount_id());
            }
        }
        return value;
    }
    Vector<Student>::Position get_student_pos() const
    {
        return m_student_pos;
    }
};