#include "calendar_day.h"

Calendar_Day::Calendar_Day(std::vector<Lesson_Info*>& lessons_in_this_day) : lessons(&lessons_in_this_day)
{
    Student_Status default_status;
    default_status.status = STATUS_NO_DATA;

    attendance_info = std::vector<std::vector<Internal_Attendance_Status>>();
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        attendance_info.push_back(std::vector<Internal_Attendance_Status>(lessons_in_this_day[i]->get_lessons_size()));
        for (int j = 0; j < lessons_in_this_day[i]->get_lessons_size(); j++)
        {
            for (int k = 0; k < lessons_in_this_day[i]->get_group().get_size(); k++)
            {
                Student_Status new_status;
                Attend_Data current_attend_data = lessons_in_this_day[i]->get_group().get_attend_data(k);
                if ((j == 0 && current_attend_data == ATTEND_SECOND) || (j == 1 && current_attend_data == ATTEND_FIRST))
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

int Calendar_Day::find_merged_lesson(const Lesson_Info& l_info) const
{
    for (int current_merged_lesson = 0; current_merged_lesson < lessons->size(); current_merged_lesson++)
    {
        if (l_info == *(lessons->at(current_merged_lesson)))
        {
            return current_merged_lesson;
        }
    }
    return -1;
}

bool Calendar_Day::add_workout(Student& student_to_workout, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to)
{
    Workout_Info new_workout_info{&student_to_workout, &merged_to, internal_to, cached_time_to};
    int internal_from = known_lesson_from.internal_lesson_id;
    int appropriate_merged_lesson = known_lesson_from.merged_lesson_id;
    for (int current_workout = 0; current_workout < attendance_info[appropriate_merged_lesson][internal_from].workouts.size(); current_workout++)
    {
        if (PTRREF(attendance_info[appropriate_merged_lesson][internal_from].workouts[current_workout].student) == student_to_workout) return false;
    }
    attendance_info[appropriate_merged_lesson][internal_from].workouts.push_back(new_workout_info);
    return true;
}

bool Calendar_Day::add_workout(int known_id_student, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to)
{
    const Student& student_to_workout = lessons->at(known_lesson_from.merged_lesson_id)->get_group().get_student(known_id_student);
    Workout_Info new_workout_info{&student_to_workout, &merged_to, internal_to, cached_time_to};
    for (int current_workout = 0; current_workout < attendance_info[known_lesson_from.merged_lesson_id][known_lesson_from.internal_lesson_id].workouts.size(); current_workout++)
    {
        if (PTRREF(attendance_info[known_lesson_from.merged_lesson_id][known_lesson_from.internal_lesson_id].workouts[current_workout].student) == student_to_workout) return false;
    }
    attendance_info[known_lesson_from.merged_lesson_id][known_lesson_from.internal_lesson_id].workouts.push_back(new_workout_info);
    return true;
}

bool Calendar_Day::add_workout(Student& student_to_workout, Lesson known_lesson_from, Workout_Info new_workout_info)
{
    attendance_info[known_lesson_from.merged_lesson_id][known_lesson_from.internal_lesson_id].workouts.push_back(new_workout_info);
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

bool Calendar_Day::set_status(Lesson known_lesson, int known_id_student, int status)
{
    attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].planned[known_id_student].status = status;
    return true;
}

bool Calendar_Day::insert_workout_into_status(Lesson known_lesson, int known_id_student, Workout_Info workout_info)
{
    attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].planned[known_id_student].workout_info = workout_info;
    return true;
}

int Calendar_Day::find_student(Student& student, int known_merged_lesson_id) const
{
    return lessons->at(known_merged_lesson_id)->get_group().find_student(student);
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

bool Calendar_Day::set_discount_status(Lesson known_lesson, int known_student_id, int discount_status)
{
    attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].planned[known_student_id].discount_status = discount_status;
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

int Calendar_Day::get_discount_status(Lesson known_lesson, int known_id_student)
{
    return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].planned[known_id_student].discount_status;
}

Student_Status Calendar_Day::get_status(const Lesson_Info& merged_lesson, int internal_lesson, const Student& student) const
{
    Student_Status error_status;
    error_status.status = STATUS_INVALID;
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return error_status;
    int student_id = merged_lesson.get_group().find_student(student);
    if (student_id == -1) return error_status;
    return attendance_info[merged_lesson_id][internal_lesson].planned[student_id];
}

Student_Status Calendar_Day::get_status(Lesson known_lesson, int known_id_student) const
{
    return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].planned[known_id_student];
}

int Calendar_Day::get_workout_size(Lesson_Info& merged_lesson, int internal_lesson) const
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return -1;
    return attendance_info[merged_lesson_id][internal_lesson].workouts.size();
}

int Calendar_Day::get_workout_size(Lesson known_lesson) const
{
    return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts.size();
}

const Student& Calendar_Day::get_workout_student(Lesson_Info& merged_lesson, int internal_lesson, int workout_id) const
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    //this must throw an exception if merged_lesson wasn't found anyway
    return PTRREF(attendance_info[merged_lesson_id][internal_lesson].workouts[workout_id].student);
}

const Student* Calendar_Day::get_workout_student(Lesson known_lesson, int workout_id) const
{
    //this must throw an exception if merged_lesson wasn't found anyway
    return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts[workout_id].student;
}

bool Calendar_Day::delete_workout(Lesson_Info& merged_lesson, int internal_lesson, const Student& student)
{
    int merged_lesson_id = find_merged_lesson(merged_lesson);
    if (merged_lesson_id == -1) return false;
    int workout_id = -1;
    for (int i = 0; i < attendance_info[merged_lesson_id][internal_lesson].workouts.size(); i++)
    {
        if (PTRREF(attendance_info[merged_lesson_id][internal_lesson].workouts[i].student) == student) workout_id = i;
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
    for (int i = 0; i < lessons->size(); i++)
    {
        if (lessons->at(i)->get_group()==group)
        {
            for (int j = 0; j < lessons->at(i)->get_lessons_size(); j++)
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
    for (int j = 0; j < lessons->at(known_merged_lesson_id)->get_lessons_size(); j++)
    {
        attendance_info[known_merged_lesson_id][j].planned.insert(attendance_info[known_merged_lesson_id][j].planned.begin() + known_new_student_id, empty_status);
    }
    return true;
}

bool Calendar_Day::swap_merged_lessons(int old_id, int new_id)
{
    if (old_id == new_id) return false;
    auto begin = attendance_info.begin();
    auto backup = attendance_info[old_id];
    attendance_info.erase(begin + old_id);
    attendance_info.insert(begin + new_id, backup);
    return true;
}

bool Calendar_Day::add_merged_lesson(Lesson_Info& new_lesson_info, bool await_no_one, int known_new_merged_lesson_id)
{
    attendance_info.insert(attendance_info.begin() + known_new_merged_lesson_id, std::vector<Internal_Attendance_Status>(new_lesson_info.get_lessons_size()));
    for (int j = 0; j < new_lesson_info.get_lessons_size(); j++)
    {
        Student_Status new_status;
        if (await_no_one) new_status.status = STATUS_NOT_AWAITED;
        else new_status.status = STATUS_NO_DATA;
        for (int k = 0; k < new_lesson_info.get_group().get_size(); k++)
        {
            attendance_info[known_new_merged_lesson_id][j].planned[k] = new_status;
        }
    }
    return true;
}

Workout_Info Calendar_Day::get_workout_info(Lesson known_lesson, int known_workout_id) const
{
    return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts[known_workout_id];
}

Workout_Info Calendar_Day::get_workout_info(Lesson known_lesson, const Student& student) const
{
    int workout_size = attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts.size();
    for (int i = 0; i < workout_size; i++)
    {
        if (PTRREF(attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts[i].student) == student) 
        {
            return attendance_info[known_lesson.merged_lesson_id][known_lesson.internal_lesson_id].workouts[i];
        }
    }
    Workout_Info not_found(nullptr, nullptr, -1, std::tm());
    return not_found;
}