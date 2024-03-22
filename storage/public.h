#pragma once
#include "storage.h"
#include "serializable.h"

#define NAME_DRAWING 0
#define NAME_SCULPTING 1
#define NAME_DESIGN 2
#define NAME_TECHDRAWING 3
#define NAME_SPECIALCOURSE 4

namespace Journal
{
    const int LESSON_TYPE_COUNT = 5;
    const int LESSON_PRICES_COUNT = 3;
    const int AGE_GROUP_COUNT = 8;

    const time_t current_timestamp = time(NULL);
    const std::tm current_time = *std::localtime(&current_timestamp);

    const std::array<std::string, LESSON_TYPE_COUNT> Lesson_Names =
        {"ИЗО", 
        "Лепка",
        "Дизайн",
        "Черчение",
        "Спецкурс"};
    const int Lesson_Prices[LESSON_TYPE_COUNT][LESSON_PRICES_COUNT] = 
        {{100, 99, 98}, 
        {200, 199, 198}, 
        {300, 299, 298}, 
        {400, 399, 398}, 
        {500, 499, 498}};
    const std::array<std::string, 12> Month_Names = 
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
    const std::array<std::string, 7> Day_Names = 
        {"Воскресенье", 
        "Понедельник", 
        "Вторник", 
        "Среда", 
        "Четверг", 
        "Пятница", 
        "Суббота"};
    const std::array<std::string, 7> Day_Names_Abbreviated = 
        {"Вс", 
        "Пн", 
        "Вт", 
        "Ср", 
        "Чт", 
        "Пт", 
        "Сб"};
    const std::array<std::string, AGE_GROUP_COUNT> Age_Group_Names = 
        {"4 года, дошкольная группа", 
        "5 лет, дошкольная группа", 
        "6 лет, дошкольная группа", 
        "7 лет, школьная группа", 
        "8 лет, школьная группа", 
        "9 лет, школьная группа", 
        "10-11 лет, школьная группа", 
        "12-13 лет, школьная группа"};
    const Student* student(int id);
    const int student_count();
    const Lesson_Info* lesson_info(int wday, int merged_lesson_id);
    const int lesson_info_count(int wday);
    const Calendar_Day* day(int mday);
    const int day_count();
    const int wday(int mday);
    const int current_year();
    const int current_month();
    const int current_day_of_the_week();
    const int current_month_days_num();
    void set_date(int month, int year);
    void add_student_to_base(std::string name, int contract);
    void add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs);
    void add_student_to_group(int student_id, int wday, int merged_lesson_id);
    void add_working_out(const std::tm caller_date, const std::tm select_date, int student_id, Lesson caller_lesson, Lesson select_lesson);
    void edit_lesson(int wday, int merged_lesson_id, int number, std::string comment, std::vector<Lesson_Pair> pairs);
    bool is_workout_possible(std::tm date, int student_id, int caller_lesson_name_id);
}