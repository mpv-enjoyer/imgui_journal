#pragma once
#include "storage.h"
#include "serializable.h"

const std::array<std::string, LESSON_TYPE_COUNT> _lesson_names =
    {"ИЗО", 
    "Лепка",
    "Дизайн",
    "Черчение",
    "Спецкурс"};
const int _lesson_prices[LESSON_TYPE_COUNT][LESSON_PRICES_COUNT] = 
    {{100, 99, 98}, 
    {200, 199, 198}, 
    {300, 299, 298}, 
    {400, 399, 398}, 
    {500, 499, 498}};
const std::array<std::string, 12> _month_names = 
    {"Январь", 
    "Февраль", 
    "Март", 
    "Апрель", 
    "Май", 
    "Июнь", 
    "Июль", 
    "Август", 
    "Сентябрь", 
    "Октябрь", 
    "Ноябрь", 
    "Декабрь"};
const std::array<std::string, 7> _day_names = 
    {"Воскресенье", 
    "Понедельник", 
    "Вторник", 
    "Среда", 
    "Четверг", 
    "Пятница", 
    "Суббота"};
const std::array<std::string, 7> _day_names_abbreviated = 
    {"Вс", 
    "Пн", 
    "Вт", 
    "Ср", 
    "Чт", 
    "Пт", 
    "Сб"};
const std::array<std::string, AGE_GROUP_COUNT> _age_group_names = 
    {"4 года, дошкольная группа", 
    "5 лет, дошкольная группа", 
    "6 лет, дошкольная группа", 
    "7 лет, школьная группа", 
    "8 лет, школьная группа", 
    "9 лет, школьная группа", 
    "10-11 лет, школьная группа", 
    "12-13 лет, школьная группа"};

const time_t _current_timestamp = time(NULL);
const std::tm _current_time = *std::localtime(&_current_timestamp);
static int _current_year;
static int _current_month;
static int _current_month_days_num;
std::vector<_Day_With_Info> _enumerate_days(int day_of_the_week);
Calendar_Day* _day(int mday);
int _discount_status(int student_contract);
int _emplace_lesson_info(int wday, Lesson_Info& lesson_info);