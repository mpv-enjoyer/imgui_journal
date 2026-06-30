#pragma once
#include <set>
#include <memory>
#include <optional>
#include "common/jtime.h"
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
    std::set<Holiday> get_holidays() const
    {
        return m_holidays;
    }
    void add_holiday(Holiday holiday)
    {
        m_holidays.insert(holiday);
    }
    void remove_holiday(Mday mday)
    {
        m_holidays.erase(Holiday(mday));
    }
};