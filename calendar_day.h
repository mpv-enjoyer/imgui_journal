#pragma once
#include "main.h"

class Calendar_Day
{
private:
    std::vector<Lesson_Info&>& lessons;
    std::vector<std::vector<Internal_Attendance_Status>> attendance_info;
public:
    Calendar_Day::Calendar_Day(std::vector<Lesson_Info&> lessons_in_this_day); //LESSONS MUST BE SORTED.
    bool set_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int status);
    Student_Status get_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student) const;
    bool add_workout(Student& student_to_workout, Lesson_Info& merged_from, int internal_from, Lesson_Info& merged_to, int internal_to);
    bool add_workout(Lesson lesson, Workout_Info new_workout_info);
    int get_workout_size(Lesson_Info& merged_lesson, int internal_lesson);
    Student& get_workout_student(Lesson_Info& merged_lesson, int internal_lesson, int workout_id);
    Workout_Info get_workout_info(Lesson lesson, int student_id);
    bool delete_workout(Lesson_Info& merged_lesson, int internal_lesson, Student& student);
    bool set_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int discount_status);
    int get_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student);
    int find_merged_lesson(Lesson_Info& l_info) const;
    int find_student(Student& student, int known_merged_lesson_id, int known_internal_lesson_id);
    //the following is needed to properly update the journal

    bool add_student_to_group(int group_id, int student_id, int new_student_id);
    bool delete_student_from_group(int group_id, int student_id); //not needed?
    bool change_group(Lesson lesson, int new_group_id);
    bool change_lesson_pair(Lesson lesson, Lesson_Pair new_lesson_pair);
    bool add_merged_lesson(int day_of_the_week, Lesson_Info new_lesson_info, bool await_no_one, int merged_lesson_id);
    //...
};
