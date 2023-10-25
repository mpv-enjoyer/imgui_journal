#pragma once
#include "main_types.h"

struct Workout_Info
{
    Student* student;
    Lesson_Info* lesson_info;
    int internal_lesson;
    std::tm cached_date;
};

struct Internal_Attendance_Status
{
    std::vector<Student_Status> planned;
    std::vector<Workout_Info>  workouts;
};

class Calendar_Day
{
private:
    std::vector<Lesson_Info*>& lessons;
    std::vector<std::vector<Internal_Attendance_Status>> attendance_info;
public:
    Calendar_Day(std::vector<Lesson_Info*>& lessons_in_this_day); //LESSONS MUST BE SORTED BEFORE CALLING.
    bool set_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int status);
    bool set_status(Lesson known_lesson, int known_id_student, int status);
    Student_Status get_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student) const;
    Student_Status get_status(Lesson known_lesson, int known_id_student) const;
    bool add_workout(Student& student_to_workout, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to);
    bool add_workout(int known_id_student, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to);
    bool add_workout(Student& student_to_workout, Lesson known_lesson_from, Workout_Info new_workout_info);
    int get_workout_size(Lesson_Info& merged_lesson, int internal_lesson);
    int get_workout_size(Lesson known_lesson);
    const Student& get_workout_student(Lesson_Info& merged_lesson, int internal_lesson, int known_workout_id);
    Student* get_workout_student(Lesson known_lesson, int known_workout_id);
    Workout_Info get_workout_info(Lesson known_lesson, int known_workout_id);
    Workout_Info get_workout_info(Lesson known_lesson, const Student& student);
    bool delete_workout(Lesson_Info& merged_lesson, int internal_lesson, Student& student);
    bool delete_workout(Lesson known_lesson, int known_workout_id);
    bool set_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int discount_status);
    bool set_discount_status(Lesson known_lesson, int internal_lesson, Student& student, int discount_status);
    bool set_discount_status(Lesson known_lesson, int known_student_id, int discount_status);
    int get_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student);
    int get_discount_status(Lesson known_lesson, int known_id_student);
    int find_merged_lesson(Lesson_Info& l_info) const;
    int find_student(Student& student, int known_merged_lesson_id);
    //the following is needed to properly update the journal
    bool add_student_to_group(Group& group, Student& new_student, int known_new_student_id);
    bool add_student_to_group(int known_merged_lesson_id, Student& new_student, int known_new_student_id);
    bool delete_student_from_group(int group_id, int student_id); //not needed?
    bool change_group(Lesson lesson, int new_group_id);
    bool change_lesson_pair(Lesson lesson, Lesson_Pair new_lesson_pair);
    bool add_merged_lesson(Lesson_Info& new_lesson_info, bool await_no_one, int known_new_merged_lesson_id);
    //...
};

struct Visible_Day
{
    int number; //WARNING: THIS STARTS WITH 1
    Calendar_Day* day;
    bool is_future;
    bool is_today;
};