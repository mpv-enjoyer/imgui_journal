#include "main.h"

#define STATUS_NOT_AWAITED -1
#define STATUS_NO_DATA      0
#define STATUS_ON_LESSON    1
#define STATUS_WAS_ILL      2
#define STATUS_SKIPPED      3
#define STATUS_WORKED_OUT   4

Calendar_Day::Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students)
{
    Calendar_Day::lessons = lessons_in_this_day;
    Calendar_Day::all_groups = all_groups;
    Calendar_Day::all_students = all_students;
    int max_merged = 0;
    for (int i = 0; i < lessons_in_this_day->size(); i++)
    {
        if (lessons_in_this_day->at(i).get_lessons_size() > max_merged) max_merged = lessons_in_this_day->at(i).get_lessons_size();
    }
    Student_Status default_status;
    default_status.status = STATUS_NO_DATA;
    
    std::vector<std::vector<std::vector<Student_Status>>> student_status(lessons_in_this_day->size(), std::vector<std::vector<Student_Status>>(max_merged, std::vector<Student_Status>()));
    std::vector<std::vector<std::vector<Workout_Info>>> workouts(lessons_in_this_day->size(), std::vector<std::vector<Workout_Info>>(max_merged, std::vector<Workout_Info>()));
    for (int i = 0; i < lessons_in_this_day->size(); i++)
    {
        for (int j = 0; j < lessons_in_this_day->at(i).get_lessons_size(); j++)
        {
            for (int k = 0; k < all_groups->at(lessons_in_this_day->at(i).get_lesson_pair(j).lesson_name_id).get_size(); k++)
            {
                if (all_students->at(all_groups->at(lessons_in_this_day->at(i).get_lesson_pair(j).lesson_name_id).get_student_sort_id(k)).is_ignored(i, j))
                {
                    student_status[i][j][k].status = STATUS_NOT_AWAITED;
                }
            }
        }
    }
}

bool Calendar_Day::add_workout(int merged_lesson_id, int internal_lesson_id, int student_id, std::tm workout_date, int workout_merged_lesson, int workout_internal_lesson)
{
    Workout_Info new_workout_info;
    new_workout_info.date = workout_date;
    new_workout_info.internal_lesson = workout_internal_lesson;
    new_workout_info.merged_lesson = workout_merged_lesson;
    new_workout_info.student_id = student_id;
    workouts[merged_lesson_id][internal_lesson_id].push_back(new_workout_info);
    return true;
}

