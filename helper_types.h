#pragma once
#include "main.h"

struct JTime //used separately with ctime.
{
    int hours; //0-23
    int minutes; //0-59
};

struct Group_Pair
{
    int day_of_the_week;
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

/*struct Lesson_Ignore
{
    Lesson_Info& merged_lesson;
    int internal_lesson_id;
};*/

struct Student_Status
{
    int status;
    std::tm workout_day;
    Lesson workout_lesson;
    int discount_status = -1;
};

inline bool operator==(const Lesson& lhs, const Lesson& rhs) { return lhs.internal_lesson_id == rhs.internal_lesson_id && lhs.merged_lesson_id == rhs.merged_lesson_id; }
inline bool operator!=(const Lesson& lhs, const Lesson& rhs) { return !(lhs == rhs); }
inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

//Popups & secondary windows
//static int popup_edit_ignore_lessons_is_open = -1;
//bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups, int* popup_edit_ignore_lessons_is_open);
//bool popup_edit_ignore_lessons(std::vector<std::vector<Lesson_Info>>* lessons_in_a_week, std::vector<Student>* all_students, int current_student_id, bool* ignore);

