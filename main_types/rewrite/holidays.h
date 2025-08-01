#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "lib_time.h"
#include "holiday.h"

class Holidays
{
    std::vector<Holiday> m_holidays;
public:
    Holidays() { }
    std::optional<Holiday> get_holiday(Mday mday) const
    {
        for (const auto& holiday : m_holidays)
        {
            if (holiday.get_mday() == mday) return holiday;
        }
        return {};
    }
    void add_holiday(Mday mday)
    {
        if (!get_holiday(mday))
        {
            m_holidays.push_back(Holiday(mday));
        }
    }
};