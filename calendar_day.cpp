#include "main.h"
#include "calendar_day.h"

Calendar_Day::Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students, int current_day_of_the_week)
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

    Calendar_Day::student_status = std::vector<std::vector<std::vector<Student_Status>>>(lessons_in_this_day->size(), std::vector<std::vector<Student_Status>>(max_merged, std::vector<Student_Status>()));
    Calendar_Day::workouts = std::vector<std::vector<std::vector<Workout_Info>>>(lessons_in_this_day->size(), std::vector<std::vector<Workout_Info>>(max_merged, std::vector<Workout_Info>()));
    for (int i = 0; i < lessons_in_this_day->size(); i++)
    {
        for (int j = 0; j < lessons_in_this_day->at(i).get_lessons_size(); j++)
        {
            Lesson current_lesson;
            current_lesson.merged_lesson_id = i;
            current_lesson.internal_lesson_id = j;
            for (int k = 0; k < all_groups->at(lessons_in_this_day->at(i).get_group()).get_size(); k++)
            {
                Student_Status new_status;
                
                new_status.student_id = all_groups->at(lessons_in_this_day->at(i).get_group()).get_student_sort_id(k);
                if (all_students->at(all_groups->at(lessons_in_this_day->at(i).get_group()).get_student_sort_id(k)).is_ignored(current_lesson, current_day_of_the_week))
                {
                    new_status.status = STATUS_NOT_AWAITED;
                }
                else
                {
                    new_status.status = STATUS_NO_DATA;
                }
                student_status[i][j].push_back(new_status);
            }
        }
    }
}

bool Calendar_Day::add_workout(Lesson lesson, int student_id, std::tm workout_date, Lesson_Pair workout_lesson)
{
    Workout_Info new_workout_info;
    new_workout_info.date = workout_date;
    new_workout_info.lesson_pair = workout_lesson;
    new_workout_info.student_id = student_id;
    workouts[lesson.merged_lesson_id][lesson.internal_lesson_id].push_back(new_workout_info);
    return true;
}

bool Calendar_Day::add_workout(Lesson lesson, Workout_Info new_workout_info)
{
    workouts[lesson.merged_lesson_id][lesson.internal_lesson_id].push_back(new_workout_info);
    return true;
}

bool Calendar_Day::set_status(Lesson lesson, int student_id, int status)
{
    for (int i = 0; i < all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_size(); i++)
    {
        int current_student = all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_student_sort_id(i);
        if (current_student == student_id)
        {
            student_status[lesson.merged_lesson_id][lesson.internal_lesson_id][i].status = status;
            return true;
        }
    }
    return false;
}

bool Calendar_Day::set_discount_status(Lesson lesson, int student_id, int discount_status)
{
    for (int i = 0; i < all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_size(); i++)
    {
        int current_student = all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_student_sort_id(i);
        if (current_student == student_id)
        {
            student_status[lesson.merged_lesson_id][lesson.internal_lesson_id][i].discount_status = discount_status;
            return true;
        }
    }
    return false;
}

int Calendar_Day::get_discount_status(Lesson lesson, int student_id)
{
    for (int i = 0; i < all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_size(); i++)
    {
        int current_student = all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_student_sort_id(i);
        if (current_student == student_id)
        {
            if (student_status[lesson.merged_lesson_id][lesson.internal_lesson_id][i].status <= STATUS_NO_DATA)
            {
                return -1;
            }
            return student_status[lesson.merged_lesson_id][lesson.internal_lesson_id][i].discount_status;
        }
    }
    return -1;
}

Student_Status Calendar_Day::get_status(Lesson lesson, int student_id) const
{
    for (int i = 0; i < all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_size(); i++)
    {
        int current_student = all_groups->at(lessons->at(lesson.merged_lesson_id).get_group()).get_student_sort_id(i);
        if (current_student == student_id)
        {
            return student_status[lesson.merged_lesson_id][lesson.internal_lesson_id][i];
        }
    }
    Student_Status error_status;
    error_status.status = STATUS_INVALID;
    return error_status;
}

int Calendar_Day::get_workout_size(Lesson lesson)
{
    return workouts[lesson.merged_lesson_id][lesson.internal_lesson_id].size();
}

int Calendar_Day::get_workout_student_id(Lesson lesson, int workout_id)
{
    return workouts[lesson.merged_lesson_id][lesson.internal_lesson_id][workout_id].student_id;
}

bool Calendar_Day::delete_workout(Lesson lesson, int workout_id)
{
    workouts[lesson.merged_lesson_id][lesson.internal_lesson_id].erase(workouts[lesson.merged_lesson_id][lesson.internal_lesson_id].begin() + workout_id);
    return true;
}

bool Calendar_Day::add_student_to_group(int group_id, int student_id, int new_student_id)
{
    if (new_student_id == -1) return false;
    Student_Status empty_status;
    empty_status.student_id = student_id;
    empty_status.status = STATUS_NO_DATA;
    empty_status.discount_status = -1;
    for (int i = 0; i < lessons->size(); i++)
    {
        if (lessons->at(i).get_group()==group_id)
        {
            for (int j = 0; j < lessons->at(i).get_lessons_size(); j++)
            {
                student_status[i][j].insert(student_status[i][j].begin() + new_student_id, empty_status);
            }
        }
    }
    return true;
}

bool Calendar_Day::change_group(Lesson lesson, int new_group_id) //this function is so broken LOL DO NOT USE
{
    int new_group_size = all_groups->at(new_group_id).get_size();
    lessons->at(lesson.merged_lesson_id).set_group(new_group_id);
    student_status[lesson.merged_lesson_id][lesson.internal_lesson_id].erase(student_status[lesson.merged_lesson_id][lesson.internal_lesson_id].begin(), student_status[lesson.merged_lesson_id][lesson.internal_lesson_id].end());
    
    Student_Status empty_status;
    empty_status.status = STATUS_NO_DATA;
    for (int i = 0; i < new_group_size; i++)
    {
        empty_status.student_id = all_groups->at(new_group_id).get_student_sort_id(i);
        student_status[lesson.merged_lesson_id][lesson.internal_lesson_id].push_back(empty_status);
    }
}

bool Calendar_Day::add_merged_lesson(int day_of_the_week, Lesson_Info new_lesson_info, bool await_no_one, int merged_lesson_id)
{
    std::vector<std::vector<Student_Status>> temp_1 = std::vector<std::vector<Student_Status>>(new_lesson_info.get_lessons_size(), std::vector<Student_Status>());
    student_status.push_back(temp_1);
    std::vector<std::vector<Workout_Info>> temp_2 = std::vector<std::vector<Workout_Info>>(new_lesson_info.get_lessons_size(), std::vector<Workout_Info>());
    workouts.push_back(temp_2);
    for (int j = 0; j < new_lesson_info.get_lessons_size(); j++)
    {
        Lesson current_lesson;
        current_lesson.merged_lesson_id = merged_lesson_id;
        current_lesson.internal_lesson_id = j;
        for (int k = 0; k < all_groups->at(new_lesson_info.get_group()).get_size(); k++)
        {
            Student_Status new_status;
            //new_status.student_id = all_groups->at(lessons_in_this_day->at(i).get_group()).get_student_sort_id(k);
            if (await_no_one)
            {
                new_status.status = STATUS_NOT_AWAITED;
            }
            else
            {
                new_status.status = STATUS_NO_DATA;
            }
            student_status[current_lesson.merged_lesson_id][j].push_back(new_status);
        }
    }
    return true;
}

Workout_Info Calendar_Day::get_workout_info(Lesson lesson, int student_id)
{
    int wsize = get_workout_size(lesson);
    for (int i = 0; i < wsize; i++)
    {
        if (workouts[lesson.merged_lesson_id][lesson.internal_lesson_id][i].student_id == student_id) 
        {
            return workouts[lesson.merged_lesson_id][lesson.internal_lesson_id][i];
        }
    }
    Workout_Info error_info;
    error_info.student_id = -1;
    return error_info;
}