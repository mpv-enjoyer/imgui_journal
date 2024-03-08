#pragma once
#include "storage.h"

namespace Internal_Storage
{
    time_t _current_timestamp = time(NULL);
    int _current_year;
    int _current_month;
    int _current_day_of_the_week;
    int _current_month_days_num;
    std::vector<Visible_Day> _visible_days;
}
