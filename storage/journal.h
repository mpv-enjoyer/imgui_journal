#pragma once
#include "../main_types/main_types.h"
#include "../helpers/helpers.h"
#include "workout.h"

#define NAME_DRAWING 0
#define NAME_SCULPTING 1
#define NAME_DESIGN 2
#define NAME_TECHDRAWING 3
#define NAME_SPECIALCOURSE 4

class Test_Journal
{
    void add_student();
    void add_merged_lesson();
    void add_workout();
    void discount();
    void merged_lesson_sort();
public:
    Test_Journal();
};

class Journal
{
    friend Test_Journal;
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
        {"не указан"
        "4 года, дошкольная группа", 
        "5 лет, дошкольная группа", 
        "6 лет, дошкольная группа", 
        "7 лет, школьная группа", 
        "8 лет, школьная группа", 
        "9 лет, школьная группа", 
        "10-11 лет, школьная группа", 
        "12-13 лет, школьная группа"};

    const time_t _current_timestamp = time(NULL);
    const std::tm _current_time = *std::localtime(&_current_timestamp);
    std::vector<_Day_With_Info> _enumerate_days(int day_of_the_week);
    Calendar_Day* _day(int mday);
    int _discount_status(int student_contract);
    int _emplace_lesson_info(int wday, Lesson_Info& lesson_info);
    bool _match_lesson_types(int l, int r);
    bool _search_last_generated_month(int *month, int *year);
    

    Journal* generation_base = nullptr;

    int _current_year;
    int _current_month;
    int _current_month_days_num;

    std::vector<Student*> _all_students;
    std::vector<Group*> _all_groups;
    std::vector<std::vector<Lesson_Info*>> _all_lessons;
    std::vector<Calendar_Day*> _all_days;

    Workout_Handler* _workout_handler;
    bool _load_workouts();
public:
    Journal();
    Journal(int month, int year, Journal *journal_main);

    const time_t current_timestamp = time(NULL);
    const std::tm current_time = *std::localtime(&current_timestamp);

    std::string Lesson_name(int type);
    const int Lesson_price(int type, int discount_status);
    std::string Month_name(int number);
    std::string Wday_name(int wday);
    std::string Wday_name_short(int wday);
    std::string Age_group(int number);

    Workout_Handler *workout_handler();
    const Student* student(int id);
    const int student_count();
    const Lesson_Info* lesson_info(int wday, int merged_lesson_id);
    const int lesson_info_count(int wday);
    const Calendar_Day* day(int mday);
    const int day_count();
    const int wday(int mday);
    const int current_year();
    const int current_month();
    const int current_month_days_num();
    const std::vector<Day_With_Info> enumerate_days(int wday);
    const int lesson_common_price(int contract, int lesson_type);
    const int lesson_current_price(Lesson lesson, int mday, int internal_student_id);
    const bool is_workout_possible(const Lesson_Info *select_lesson, int select_internal_lesson, int student_id, int caller_lesson_name_id);
    const std::string merged_lesson_name(int wday, int merged_lesson_id, int internal_student_id);

    void add_student_to_base(std::string name, int contract);
    void add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs);
    void add_student_to_group(int student_id, int wday, int merged_lesson_id);
    void add_working_out(const std::tm caller_date, const std::tm select_date, int student_id, Lesson caller_lesson, Lesson select_lesson);
    void edit_lesson_pairs(int wday, int merged_lesson_id, std::vector<Lesson_Pair> pairs);
    const std::vector<std::vector<std::pair<const Workout_Info_ *, const Workout_Info_ *>>> get_workout_info(int real_wday, int real_merged_lesson, std::vector<int> *student_ids);
    const std::vector<std::vector<const Workout_Info_ *>> get_workout_info(int real_wday, Lesson real_lesson, std::vector<int> *student_ids);
    const Workout_Info_ *get_workout_info(int should_mday, Lesson should_lesson, int should_student_id);
    void remove_student(int id);
    void restore_student(int id);
    void remove_lesson(int wday, int merged_lesson_id);
    void restore_lesson(int wday, int merged_lesson_id);
    void set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed);
    void set_student_name(int id, std::string name);
    void set_group_age_group(int wday, int merged_lesson_id, int age_group);
    void set_student_contract(int id, int contract);
    void set_group_number(int wday, int merged_lesson_id, int number);
    void set_group_comment(int wday, int merged_lesson_id, std::string comment);
    void set_student_attend_data(int wday, int merged_lesson_id, int internal_student_id, Attend_Data new_attend_data);

    enum State
    {
        Empty, // No file was loaded, no generation needed. (not generated year or a month earlier than first generated in current year)
        Preview, // No file was loaded, generation needed. (current year is generated, the month is bigger than last generated in current year)
        Limited, // File was loaded (earlier than current month)
        Fullaccess // File was loaded or generated (current month)
    };
    State get_state();
    // Almost every action requires certain State. State must never be changed after construction.

    static bool save_file_exists(int month, int year);
    void save_workouts();
    void save();
    bool load();
    void generate(int base_month, int base_year);
    void generate();
    ~Journal();
private:
    State _state;
    bool _check_rights(std::vector<State> states);
};