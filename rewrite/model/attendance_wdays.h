#pragma once
#include "attendance_wday.h"
#include "ids.h"

class AWdays
{
    Vector_Sortable<AWday> m_wdays;
public:
    AWdays()
    {
        for (std::size_t i = 0; i < Wday::COUNT; i++)
        {
            m_wdays.push_back(Ptr<AWday>::make());
        }
    }
    const AWday& cref_wday(Wday wday) const
    {
        return *(m_wdays.cref_data().at(wday.get_EN()));
    }
    AWday& ref_wday(Wday wday)
    {
        return *(m_wdays.ref_data().at(wday.get_EN()));
    }
};