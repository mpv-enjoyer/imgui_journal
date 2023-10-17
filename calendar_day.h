#pragma once
#include "main.h"

class Calendar_Day
{
private:
    std::vector<Lesson_Info>* lessons;
    std::vector<Group>* all_groups;
    std::vector<Student>* all_students;
    std::vector<std::vector<std::vector<Student_Status>>> student_status; //[merged_lesson][internal_lesson][student_in_group]
    //TODO: change student_status structure to ID-based? (for stability)
    std::vector<std::vector<std::vector<Workout_Info>>> workouts; //[merged_lesson][internal_lesson][new_student]
public:
    Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students, int current_day_of_the_week);
    bool set_status(Lesson lesson, int student_id, int status);
    Student_Status get_status(Lesson lesson, int student_id) const;
    bool add_workout(Lesson lesson, int student_id, std::tm workout_date, Lesson_Pair workout_lesson);
    bool add_workout(Lesson lesson, Workout_Info new_workout_info);
    int get_workout_size(Lesson lesson);
    int get_workout_student_id(Lesson lesson, int workout_id);
    Workout_Info get_workout_info(Lesson lesson, int student_id);
    bool delete_workout(Lesson lesson, int student_id);
    bool set_discount_status(Lesson lesson, int student_id, int discount_status);
    int get_discount_status(Lesson lesson, int student_id);

    //the following is needed to properly update the journal

    bool add_student_to_group(int group_id, int student_id, int new_student_id);
    bool delete_student_from_group(int group_id, int student_id); //not needed?
    bool change_group(Lesson lesson, int new_group_id);
    bool change_lesson_pair(Lesson lesson, Lesson_Pair new_lesson_pair);
    bool add_merged_lesson(int day_of_the_week, Lesson_Info new_lesson_info, bool await_no_one, int merged_lesson_id);
    //...
};
