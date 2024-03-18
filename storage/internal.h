#pragma once
#include "storage.h"
#include "serializable.h"

time_t _current_timestamp = time(NULL);
const std::tm _current_time = *std::localtime(&_current_timestamp);
int _current_year;
int _current_month;
int _current_day_of_the_week;
int _current_month_days_num;
std::vector<Day_With_Info> _visible_days;
std::vector<Day_With_Info> _enumerate_days(int day_of_the_week);
bool _is_future(int day);
bool _is_today(int day);