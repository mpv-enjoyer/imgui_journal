#pragma once
#include "storage.h"
#include "serializable.h"

const time_t _current_timestamp = time(NULL);
const std::tm _current_time = *std::localtime(&_current_timestamp);
int _current_year;
int _current_month;
int _current_month_days_num;
std::vector<_Day_With_Info> _enumerate_days(int day_of_the_week);
Calendar_Day* _day(int mday);
int _discount_status(int student_contract);
int _emplace_lesson_info(int wday, Lesson_Info& lesson_info);