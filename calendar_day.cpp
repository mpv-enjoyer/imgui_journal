#include "main.h"
#include "calendar_day.h"
#include "lesson_info.h"
#include "group.h"
#include "student.h"

Calendar_Day::Calendar_Day(std::vector<Lesson_Info&> lessons_in_this_day) : lessons(lessons_in_this_day)
{
    int max_merged = 0;
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        if (lessons_in_this_day[i].get_lessons_size() > max_merged) max_merged = lessons_in_this_day[i].get_lessons_size();
    }
    Student_Status default_status;
    default_status.status = STATUS_NO_DATA;

    attendance_info = std::vector<std::vector<Internal_Attendance_Status>>();
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        attendance_info.push_back(std::vector<Internal_Attendance_Status>(lessons_in_this_day[i].get_lessons_size()));
        for (int j = 0; j < lessons_in_this_day[i].get_lessons_size(); j++)
        {
            for (int k = 0; k < lessons_in_this_day[i].get_group().get_size(); k++)
            {
                Student_Status new_status;
                if (lessons_in_this_day[i].get_group().get_student(k).is_ignored(lessons_in_this_day[i], j))
                {
                    new_status.status = STATUS_NOT_AWAITED;
                }
                else
                {
                    new_status.status = STATUS_NO_DATA;
                }
                attendance_info[i][j].planned.push_back(new_status);
            }
        }
    }
}

int Calendar_Day::find_merged_lesson(Lesson_Info& l_info) const
{
    for (int current_merged_lesson = 0; current_merged_lesson < lessons.size(); current_merged_lesson++)
    {
        if (l_info == lessons[current_merged_lesson])
        {
            return current_merged_lesson;
        }
    }
    return -1;
}

bool Calendar_Day::add_workout(Student& student_to_workout, Lesson_Info& merged_from, int internal_from, Lesson_Info& merged_to, int internal_to)
{
    Workout_Info new_workout_info{student_to_workout, merged_to, internal_to};
    int appropriate_merged_lesson = -1;
    for (int current_merged_lesson = 0; current_merged_lesson < lessons.size(); current_merged_lesson++)
    {
        if (merged_from == lessons[current_merged_lesson] && internal_from < lessons[current_merged_lesson].get_lessons_size())
        {
            appropriate_merged_lesson = current_merged_lesson;
            break;
        }
    }
    if (appropriate_merged_lesson == -1) return false;
    for (int current_workout = 0; current_workout < attendance_info[appropriate_merged_lesson][internal_from].workouts.size(); current_workout++)
    {
        if (attendance_info[appropriate_merged_lesson][internal_from].workouts[current_workout].student == student_to_workout) return false;
    }
    attendance_info[appropriate_merged_lesson][internal_from].workouts.push_back(new_workout_info);
    return true;
}

bool Calendar_Day::add_workout(int known_id_student, int known_id_merged_from, int internal_from, Lesson_Info& merged_to, int internal_to)
{
    const Student& student_to_workout = lessons[known_id_merged_from].get_group().get_student(known_id_student);
    Workout_Info new_workout_info{student_to_workout, merged_to, internal_to};
    for (int current_workout = 0; current_workout < attendance_info[known_id_merged_from][internal_from].workouts.size(); current_workout++)
    {
        if (attendance_info[known_id_merged_from][internal_from].workouts[current_workout].student == student_to_workout) return false;
    }
    attendance_info[known_id_merged_from][internal_from].workouts.push_back(new_workout_info);
    return true;
}

bool Calendar_Day::set_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int status)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return false;
    int student_id = merged_lesson.get_group().find_student(student);
    if (student_id == -1) return false;
    attendance_info[merged_lesson_id][internal_lesson].planned[student_id].status = status;
    return true;
}

bool Calendar_Day::set_status(int known_id_merged_lesson, int internal_lesson, int known_id_student, int status)
{
    attendance_info[known_id_merged_lesson][internal_lesson].planned[known_id_student].status = status;
    return true;
}

bool Calendar_Day::set_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int discount_status)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return false;
    int student_id = merged_lesson.get_group().find_student(student);
    if (student_id == -1) return false;
    attendance_info[merged_lesson_id][internal_lesson].planned[student_id].discount_status = discount_status;
    return true;
}

int Calendar_Day::get_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return -1;
    int student_id = merged_lesson.get_group().find_student(student);
    if (student_id == -1) return -1;
    return attendance_info[merged_lesson_id][internal_lesson].planned[student_id].discount_status;
}

Student_Status Calendar_Day::get_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student) const
{
    Student_Status error_status;
    error_status.status = STATUS_INVALID;
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return error_status;
    int student_id = merged_lesson.get_group().find_student(student);
    if (student_id == -1) return error_status;
    return attendance_info[merged_lesson_id][internal_lesson].planned[student_id];
}

Student_Status Calendar_Day::get_status(int known_id_merged_lesson, int internal_lesson, int known_id_student)
{
    return attendance_info[known_id_merged_lesson][internal_lesson].planned[known_id_student];
}

int Calendar_Day::get_workout_size(Lesson_Info& merged_lesson, int internal_lesson)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return -1;
    return attendance_info[merged_lesson_id][internal_lesson].workouts.size();
}

int Calendar_Day::get_workout_size(int known_merged_lesson_id, int internal_lesson)
{
    
}

const Student& Calendar_Day::get_workout_student(Lesson_Info& merged_lesson, int internal_lesson, int workout_id)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    //this must throw an exception if merged_lesson wasn't found anyway
    return attendance_info[merged_lesson_id][merged_lesson_id].workouts[workout_id].student;
}

bool Calendar_Day::delete_workout(Lesson_Info& merged_lesson, int internal_lesson, Student& student)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return false;
    int workout_id = -1;
    for (int i = 0; i < attendance_info[merged_lesson_id][internal_lesson].workouts.size(); i++)
    {
        if (attendance_info[merged_lesson_id][internal_lesson].workouts[i].student == student) workout_id = i;
    }
    if (workout_id == -1) return false;
    attendance_info[merged_lesson_id][internal_lesson].workouts.erase(attendance_info[merged_lesson_id][internal_lesson].workouts.begin() + workout_id);
    return true;
}

bool Calendar_Day::add_student_to_group(Group& group, Student& new_student, int known_new_student_id)
{
    Student_Status empty_status;
    empty_status.status = STATUS_NO_DATA;
    empty_status.discount_status = -1;
    for (int i = 0; i < lessons.size(); i++)
    {
        if (lessons[i].get_group()==group)
        {
            for (int j = 0; j < lessons[i].get_lessons_size(); j++)
            {
                attendance_info[i][j].planned.insert(attendance_info[i][j].planned.begin() + known_new_student_id, empty_status);
            }
        }
    }
    return true;
}

bool Calendar_Day::add_student_to_group(int known_merged_lesson_id, Student& new_student, int known_new_student_id)
{
    Student_Status empty_status;
    empty_status.status = STATUS_NO_DATA;
    empty_status.discount_status = -1;
    for (int j = 0; j < lessons[known_merged_lesson_id].get_lessons_size(); j++)
    {
        attendance_info[known_merged_lesson_id][j].planned.insert(attendance_info[known_merged_lesson_id][j].planned.begin() + known_new_student_id, empty_status);
    }
    return true;
}

/*bool Calendar_Day::change_group(Lesson lesson, int new_group_id) //this function is so broken LOL DO NOT USE
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
}*/

bool Calendar_Day::add_merged_lesson(Lesson_Info& new_lesson_info, bool await_no_one, int known_merged_lesson_id)
{
    attendance_info.push_back(std::vector<Internal_Attendance_Status>(new_lesson_info.get_lessons_size()));
    for (int j = 0; j < new_lesson_info.get_lessons_size(); j++)
    {
        Student_Status new_status;
        if (await_no_one) new_status.status = STATUS_NOT_AWAITED;
        else new_status.status = STATUS_NO_DATA;
        for (int k = 0; k < new_lesson_info.get_group().get_size(); k++)
        {
            attendance_info[known_merged_lesson_id][j].planned[k] = new_status;
        }
    }
    return true;
}

Workout_Info Calendar_Day::get_workout_info(int known_merged_lesson_id, int internal_lesson_id, int student_id)
{
    int workout_size = get_workout_size(lesson);
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