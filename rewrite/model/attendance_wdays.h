#pragma once
#include "attendance_wday.h"

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
    const Ptr<Attendance_Wday>& cref_wday(Wday wday) const
    {
        return m_wdays.cref_data().at(wday.get_EN());
    }
    Ptr<Attendance_Wday>& ref_wday(Wday wday)
    {
        return m_wdays.ref_data().at(wday.get_EN());
    }
};