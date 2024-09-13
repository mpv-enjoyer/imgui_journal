#include "date.h"
#include "helpers.h"

MdayFrom1 get_number_of_days(MonthFrom0 month, YearFrom0 year)
{
    month.value++;
    // leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    // months which have 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

int get_first_mwday(int month, int year)
{
    return get_wday(0, month, year);
}

int get_first_wday(int month, int year, int wday)
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

int get_wday(int day, int month, int year)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      day + 1, month, year }; // 1-based day, 0-based month, year since 1900
  std::time_t time_temp = std::mktime(&time_in);
  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm * time_out = std::localtime(&time_temp);
  return time_out->tm_wday;
}

int get_mday_index_for_wday(int mday, int wday, int month, int year)
{
    int day = get_first_wday(month, year, wday) - MDAY_DIFF;
    int day_count = get_number_of_days(month, year + 1900);
    for (int i = 0; day <= day_count; day += 7, i++)
    {
        if (day == mday) return i;
    }
    return -1;
};

int get_wday_count_in_month(int wday, int month, int year)
{
    int output = 0;
    int day = get_first_wday(month, year, wday);
    int day_count = get_number_of_days(month, year);
    for ( ; day <= day_count; day+=7, output++) { };
    return output;
}

void next_month_for(int& month, int& year)
{
    if (month == 11)
    {
        month = 0;
        year = year + 1;
    }
    else
    {
        month += 1;
    }
}

void previous_month_for(int& month, int& year)
{
    if (month == 0)
    {
        month = 11;
        year = year - 1;
    }
    else
    {
        month -= 1;
    }
}