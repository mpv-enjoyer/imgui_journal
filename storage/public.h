#pragma once
#include "storage.h"
#include "serializable.h"

namespace Journal
{
    const int LESSON_TYPE_COUNT = 5;
    const int LESSON_PRICES_COUNT = 3;
    const int AGE_GROUP_COUNT = 8;

    const time_t current_timestamp = time(NULL);
    const std::tm current_time = *std::localtime(&current_timestamp);

    const std::array<std::string, LESSON_TYPE_COUNT> Lesson_Names = {"ИЗО", "Лепка", "Дизайн", "Черчение", "Спецкурс"};
    const int Lesson_Prices[LESSON_TYPE_COUNT][LESSON_PRICES_COUNT] = {{100, 99, 98}, {200, 199, 198}, {300, 299, 298}, {400, 399, 398}, {500, 499, 498}};
    const std::array<std::string, 12> Month_Names = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    const std::array<std::string, 7> Day_Names = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
    const std::array<std::string, 7> Day_Names_Abbreviated = {"Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
    const std::array<std::string, AGE_GROUP_COUNT> Age_Group_Names = {"4 года, дошкольная группа", "5 лет, дошкольная группа", "6 лет, дошкольная группа", "7 лет, школьная группа", "8 лет, школьная группа", "9 лет, школьная группа", "10-11 лет, школьная группа", "12-13 лет, школьная группа"};
    
    const int current_year();
    const int current_month();
    const int current_day_of_the_week();
    const int current_month_days_num();
    const std::vector<Day_With_Info>& visible_days();
    void set_date(int month, int year);
    void set_day_of_the_week(int day_of_the_week);
    void add_student_to_base(std::string name, int contract);
    void add_merged_lesson(int day_of_the_week, int number, std::string comment, std::vector<Lesson_Pair> lesson_pairs);
}

 //const std::vector<Student*>& all_students();
 //const std::vector<Group*>& all_groups();
 //const std::vector<std::vector<Lesson_Info*>>& all_lessons();
 //const std::vector<Calendar_Day*>& all_days();
 //std::vector<Student*>& edit_all_students();
 //std::vector<Group*>& edit_all_groups();
 //std::vector<std::vector<Lesson_Info*>>& edit_all_lessons();
 //std::vector<Calendar_Day*>& edit_all_days();