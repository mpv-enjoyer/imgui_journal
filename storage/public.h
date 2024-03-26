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

    std::string Lesson_name(int type);
    const int Lesson_price(int type, int discount_status);
    std::string Month_name(int number);
    std::string Wday_name(int wday);
    std::string Wday_name_short(int wday);
    std::string Age_group(int number);

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
    const std::vector<const Day_With_Info> enumerate_days(int wday);
    const int lesson_common_price(int contract, int lesson_type);
    const int lesson_current_price(Lesson lesson, int mday, int internal_student_id);
    const bool is_workout_possible(const Lesson_Info *select_lesson, int select_internal_lesson, int student_id, int caller_lesson_name_id);
    const std::string merged_lesson_name(int wday, int merged_lesson_id, int internal_student_id);
    void set_date(int month, int year);
    void add_student_to_base(std::string name, int contract);
    void add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs);
    void add_student_to_group(int student_id, int wday, int merged_lesson_id);
    void add_working_out(const std::tm caller_date, const std::tm select_date, int student_id, Lesson caller_lesson, Lesson select_lesson);
    void edit_lesson(int wday, int merged_lesson_id, int number, std::string comment, std::vector<Lesson_Pair> pairs);
    void append_workout_students(Day_With_Info visible_day, Lesson lesson, std::vector<const Student *> &workout_students);
    void remove_student(int id);
    void restore_student(int id);
    void remove_lesson(int wday, int merged_lesson_id);
    void restore_group(int wday, int merged_lesson_id);
    void set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed);
    void set_student_name(int id, std::string name);
    void set_student_age_group(int id, int age_group);
    void set_student_contract(int id, int contract);
    void set_group_number(int wday, int merged_lesson_id, int number);
    void set_group_comment(int wday, int merged_lesson_id, std::string comment);
    void set_student_attend_data(int wday, int merged_lesson_id, int internal_student_id, Attend_Data new_attend_data);
}