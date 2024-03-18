#include "internal.h"

std::vector<Day_With_Info> _enumerate_days(int day_of_the_week)
{
    std::vector<Day_With_Info> output;
    int day = get_first_wday(_current_month, _current_year, day_of_the_week);
    int day_count = get_number_of_days(_current_month, _current_year);
    for ( ; day <= day_count; day+=7)
    {
        Day_With_Info current;
        current.day = _all_days[day - MDAY_DIFF];
        current.number = day;
        current.is_future = day > _current_time.tm_mday;
        current.is_today = day == _current_time.tm_mday;
        output.push_back(current);
    }
    return output;
}

