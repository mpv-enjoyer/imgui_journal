#pragma once
#include "../main.h"

struct JTime //used separately with ctime.
{
    int hours; //0-23
    int minutes; //0-59
};

struct Group_Pair
{
    int day_of_the_week; //not sure if it's used anywhere
    int number;
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
};

struct Lesson_Full
{
    Lesson lesson;
    int day_of_the_week;
};

inline bool operator==(const Lesson& lhs, const Lesson& rhs) { return lhs.internal_lesson_id == rhs.internal_lesson_id && lhs.merged_lesson_id == rhs.merged_lesson_id; }
inline bool operator!=(const Lesson& lhs, const Lesson& rhs) { return !(lhs == rhs); }
inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

const char* c_str_int(int num);
std::string to_string(JTime value);
std::string to_string(JTime begin, JTime end);
std::string to_string(std::tm day, JTime begin, JTime end = {-1, -1}, bool abbreviate = true);
std::string to_string(const std::vector<std::string>& strings, std::string separator = "", const std::vector<bool>& show_only = {});
std::string generate_label(const std::string prefix, std::vector<int> unique);
bool j_button_selectable(const char* label, bool selected, bool small = false);
bool j_button_dangerous(const char *label);
bool j_input_time(std::string label, JTime &time);
bool j_attendance_combo(const char* label, int* status, std::string tooltip = "");
bool j_attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name);
template <typename T = int>
bool is_in_vector(std::vector<T> vector, T to_find);
int calculate_first_mwday(int current_mday, int current_wday);
int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year);
int get_wday(int day, int month, int year);
void HelpMarker(const char* desc);

template<class Archive>
void serialize(Archive & ar, std::tm & g, const unsigned int version)
{
    ar & g.tm_gmtoff;
    ar & g.tm_hour;
    ar & g.tm_isdst;
    ar & g.tm_mday;
    ar & g.tm_min;
    ar & g.tm_mon;
    ar & g.tm_sec;
    ar & g.tm_wday;
    ar & g.tm_yday;
    ar & g.tm_year;
    ar & g.tm_zone;
}