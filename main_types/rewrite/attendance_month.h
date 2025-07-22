#pragma once
#include "attendance_holder.h"
#include "lib_vector.h"
#include "lib_time.h"

class Attendance_Month
{
    std::vector<Attendance_Holder> m_mday_statuses;
public:
    Attendance_Month(Month month, Wday wday)
    : m_mday_statuses(month.calculate_wday_count(wday))
    { }
    Attendance_Holder& attendance_holder_unchecked_mut(std::size_t index)
    {
        return m_mday_statuses.at(index);
    }
    const Attendance_Holder& attendance_holder_unchecked(std::size_t index) const
    {
        return m_mday_statuses.at(index);
    }
};