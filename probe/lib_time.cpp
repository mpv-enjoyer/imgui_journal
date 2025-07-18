#include "lib_time.h"

int Month::calculate_wday_count(Wday wday) const
{
    //int get_wday_count_in_month(int wday, int month, int year)
    return (get_day_count() - Mday::make_from_first_wday(wday, *this).get_from_1()) / Wday::COUNT + 1;
}

Mday Mday::make_from_first_wday(Wday wday, Month month)
{
    // int get_first_wday(int month, int year, int wday)
    std::tm time_in = { 0, 0, 0, // second, minute, hour
        1, month.get_from_0(), month.get_year().get_from_1900() }; // 1-based day, 0-based month, year since 1900
    std::time_t time_temp = std::mktime(&time_in);
    //Note: Return value of localtime is not threadsafe, because it might be
    // (and will be) reused in subsequent calls to std::localtime!
    const std::tm time_out = *std::localtime(&time_temp);
    int first_mday_wday = time_out.tm_wday;
    int diff = ( ( wday.get_EN() - first_mday_wday ) + 7 ) % 7;
    return Mday(diff, month); // TODO: TEST THIS
}

int Mday::get_index_in_month()
{
    return get_from_0() / Wday::COUNT;
}
