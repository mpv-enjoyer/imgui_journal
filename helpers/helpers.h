#pragma once
#include "../main.h"

struct JTime //used separately with ctime.
{
    int hours; //0-23
    int minutes; //0-59
};

struct Group_Pair
{
    int number;
    int age_group;
};

struct Lesson_Pair
{
    JTime time_begin;
    JTime time_end;
    int lesson_name_id;
};

struct Lesson
{
    int merged_lesson_id;
    int internal_lesson_id;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
    ar & merged_lesson_id;
    ar & internal_lesson_id;
}
};

struct Lesson_Full
{
    Lesson lesson;
    int day_of_the_week;
    Lesson_Full() {};
};

struct Time_Archiver
{
    int year;
    int month;
    int mday;
    int wday;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
    ar & mday;
    ar & month;
    ar & year;
    ar & wday;
}
    Time_Archiver() { };
    Time_Archiver(std::tm tm);
    std::tm to_tm();
};

inline bool operator==(const Lesson& lhs, const Lesson& rhs) { return lhs.internal_lesson_id == rhs.internal_lesson_id && lhs.merged_lesson_id == rhs.merged_lesson_id; }
inline bool operator!=(const Lesson& lhs, const Lesson& rhs) { return !(lhs == rhs); }
inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

inline bool operator==(const std::tm& lhs, const std::tm& rhs) { return lhs.tm_mday == rhs.tm_mday && lhs.tm_mon == rhs.tm_mon && lhs.tm_year == rhs.tm_year; }
inline bool operator!=(const std::tm& lhs, const std::tm& rhs) { return !(lhs == rhs); }
inline bool operator< (const std::tm& lhs, const std::tm& rhs)
{
    if (lhs.tm_year != rhs.tm_year) return lhs.tm_year < rhs.tm_year;
    if (lhs.tm_mon != rhs.tm_mon) return lhs.tm_mon < rhs.tm_mon;
    return lhs.tm_mday < rhs.tm_mday;
}
inline bool operator> (const std::tm& lhs, const std::tm& rhs) { return rhs < lhs; }
inline bool operator<=(const std::tm& lhs, const std::tm& rhs) { return !(lhs > rhs); }
inline bool operator>=(const std::tm& lhs, const std::tm& rhs) { return !(lhs < rhs); }

const char* c_str_int(int num);
std::string to_string(JTime value);
std::string to_string(JTime begin, JTime end);
std::string to_string(std::tm day, JTime begin, JTime end = {-1, -1}, bool abbreviate = true);
std::string to_string(const std::vector<std::string>& strings, std::string separator = "", const std::vector<bool>& show_only = {});
std::string generate_label(const std::string prefix, std::vector<int> unique);
bool j_age_group_combo(const char *label, int *age_group);
bool j_button_selectable(const char *label, bool selected, bool small = false);
bool j_button_dangerous(const char *label);
int validate_time_int(std::string input_string, int upper_limit);
bool j_input_time(std::string label, JTime &time);
bool j_attendance_combo(const char* label, int* status, std::string tooltip = "");
bool j_attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name);
template <typename T = int>
bool is_in_vector(std::vector<T> vector, T to_find);
int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year_starting_from_zero);
int get_first_mwday(int month, int year);
int get_wday(int day, int month, int year);
int get_mday_index_for_wday(int mday, int wday, int month, int year);
int get_wday_count_in_month(int wday, int month, int year);
void next_month_for(int& month, int& year);
void previous_month_for(int& month, int& year);
void HelpMarker(const char *desc);