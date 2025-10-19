#pragma once
#include "attendance_holder.h"
#include "removal_info.h"
#include "student.h"

class Attendance_Student
{
    NON_COPYABLE(Attendance_Student);
    const Vector_Sortable<Student>::Position m_student_pos;
    bool m_wants_lesson = true;
    std::vector<Ptr<Attendance_Holder>> m_holders;
public:
    Attendance_Student(std::size_t holders_count, Vector_Sortable<Student>::Position student_pos)
    : m_student_pos(student_pos)
    {
        for (std::size_t i = 0; i < holders_count; i++)
        {
            m_holders.push_back(Ptr<Attendance_Holder>::make());
        }
    }
    bool get_wants_lesson() const
    {
        return m_wants_lesson;
    }
    void set_wants_lesson(bool value)
    {
        m_wants_lesson = value;
    }
    const Attendance_Holder& cref_holder(Aday aday) const
    {
        return *(m_holders[aday.index()]);
    }
    Attendance_Holder& ref_holder(Aday aday)
    {
        return *(m_holders[aday.index()]);
    }
    //Attendance_Holder get_holder(Aday aday) const
    //{
    //    return cref_holder(aday);
    //}
    //Attendance_Holder get_holder_checked(Aday aday) const
    //{
    //    auto holder = get_holder(aday);
    //    if (holder.get_status() == Attendance_Status::NO_DATA)
    //    {
    //        if (!m_wants_lesson)
    //        {
    //            holder.set(Attendance_Status::NOT_AWAITED, holder.get_discount_id());
    //        }
    //    }
    //    return holder;
    //}
    Vector_Sortable<Student>::Position get_student_pos() const
    {
        return m_student_pos;
    }
};