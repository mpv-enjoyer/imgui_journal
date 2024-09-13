#include "date.h"
#include "helpers.h"

MdayFrom0 get_number_of_days(MonthFrom0 _month, YearFrom0 year)
{
    MonthFrom1 month = _month;
    // leap year condition, if month is 2
    if (MonthFrom1(month) == MonthFrom1(2)) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    // months which have 31 days
    else if (month == MonthFrom1(1) || month == MonthFrom1(3) || month == MonthFrom1(5) || month == MonthFrom1(7)
          || month == MonthFrom1(8) || month == MonthFrom1(10) || month == MonthFrom1(12))
        return 31;
    else
        return 30;
}

WdayEnglish get_first_mwday(MonthFrom0 month, YearFrom1900 year)
{
    return get_wday(MdayFrom0(0), month, year);
}

MdayFrom1 get_first_wday(MonthFrom0 month, YearFrom1900 year, WdayEnglish wday)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      1, month, year}; // 1-based day, 0-based month, year since 1900

  std::time_t time_temp = std::mktime(&time_in);

  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm time_out = *std::localtime(&time_temp);
  int first_mday_wday = time_out.tm_wday;
  int diff = ( ( wday - first_mday_wday ) + 7 ) % 7;
  return diff + 1;
}

WdayEnglish get_wday(MdayFrom0 day, MonthFrom0 month, YearFrom1900 year)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      day + 1, month, year }; // 1-based day, 0-based month, year since 1900
  std::time_t time_temp = std::mktime(&time_in);
  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm * time_out = std::localtime(&time_temp);
  return time_out->tm_wday;
}

MdayIDWday get_mday_index_for_wday(MdayFrom1 mday, WdayEnglish wday, MonthFrom0 month, YearFrom1900 year)
{
    MdayFrom1 day = get_first_wday(month, year, wday);
    MdayFrom0 day_count = get_number_of_days(month, year);
    for (int i = 0; day < MdayFrom1(day_count); day.value += 7, i++)
    {
        if (day == mday) return i;
    }
    IM_ASSERT(false && "Mday index for wday isn't valid.");
};

int get_wday_count_in_month(WdayEnglish wday, MonthFrom0 month, YearFrom1900 year)
{
    int output = 0;
    MdayFrom1 day = get_first_wday(month, year, wday);
    MdayFrom0 day_count = get_number_of_days(month, year);
    for ( ; day <= MdayFrom1(day_count); day.value += 7, output++) { };
    return output;
}

void next_month_for(MonthFrom0& month, YearFrom1900& year)
{
    if (month == MonthFrom0(11))
    {
        month = 0;
        year = year + 1;
    }
    else
    {
        month = month + 1;
    }
}

void previous_month_for(MonthFrom0& month, YearFrom1900& year)
{
    if (month == MonthFrom0(0))
    {
        month = 11;
        year = year - 1;
    }
    else
    {
        month = month - 1;
    }
}