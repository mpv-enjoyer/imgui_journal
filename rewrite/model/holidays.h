#pragma once
#include <set>
#include <memory>
#include <optional>
#include "jtime.h"
#include "holiday.h"

class Holidays
{
    std::set<Holiday> m_holidays;
public:
    Holidays() { }
    std::optional<Holiday> get_holiday(Mday mday) const
    {
        if (auto found = m_holidays.find(Holiday(mday)); found != m_holidays.end())
        {
            return *found;
        }
        return {};
    }
    void add_holiday(Mday mday)
    {
        m_holidays.insert(Holiday(mday));
    }
};