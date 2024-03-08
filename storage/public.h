#pragma once
#include "storage.h"
#include "internal.h"
#include "serializable.h"

namespace Journal
{
    const int& current_year = std::ref(_current_year);
    const int& current_month = std::ref(_current_month);
    const int& current_day_of_the_week = std::ref(_current_day_of_the_week);
    const int& current_month_days_num = std::ref(_current_month_days_num);
    const std::vector<Visible_Day>& visible_days = std::ref(_visible_days);
    const std::tm current_time = *std::localtime(&_current_timestamp);
    void set_date(int month, int year)
    {
        if (month != current_time.tm_mon || year != current_time.tm_year) throw std::invalid_argument("not implemented");
        //TODO CRITICAL: check if journal is available for this month and load.
        _current_month = month;
        _current_year = year;
        _current_month_days_num = get_number_of_days(month, year + 1900);
    }
    void set_day_of_the_week(int day_of_the_week)
    {
        _current_day_of_the_week = day_of_the_week;
        int first_visible_day = get_first_wday(_current_month, _current_year, day_of_the_week);
        _visible_days.clear();
        for (;first_visible_day <= _current_month_days_num; first_visible_day+=7)
        {
            bool is_future = current_time.tm_mday < first_visible_day;
            bool is_today = current_time.tm_mday == first_visible_day;
            Calendar_Day* current_visible = all_days[first_visible_day-MDAY_DIFF];
            _visible_days.push_back(Visible_Day{first_visible_day, current_visible, is_future, is_today});
        }
    }
}