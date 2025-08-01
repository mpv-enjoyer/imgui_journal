#pragma once
#include "attendance_wday.h"

class Attendance_Wdays
{
    Vector<Attendance_Wday> m_wdays;
public:
    Attendance_Wdays()
    : m_wdays(Wday::COUNT)
    { }
    const std::unique_ptr<Attendance_Wday>& cref_attendance_wday(Wday wday) const
    {
        return m_wdays.cref_data().at(wday.get_EN());
    }
    AUTOMUT1(ref_attendance_wday, Wday)
};