#include "group.h"

Student& Group::get_student(int known_student_id)
{
    return PTRREF(students[known_student_id].student);
}

int Group::get_number() const
{
    return group_info.number;
}

bool Group::set_number(int new_number)
{
    group_info.number = new_number;
    return true;
}

int Group::get_size() const
{
    return students.size();
}

int Group::get_day_of_the_week() const
{
    return group_info.day_of_the_week;
}

bool Group::set_day_of_the_week(int new_day)
{
    if (new_day < -1 || new_day >= 7) return false;
    group_info.day_of_the_week = new_day;
    return true;
}

int Group::find_student(Student& student) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (PTRREF(students[i].student) == student) return i;
    }
    return -1;
}

int Group::add_student(Student& new_student) //returns his internal id
{
    int new_student_sort_by_name_id = students.size(); //ascending
    for (int i = 0; i < (students.size()); i++)
    {
        if (new_student == PTRREF(students[i].student))
        {
            return -1; //already in the list
        }
        if (new_student < PTRREF(students[i].student))
        {
            new_student_sort_by_name_id = i; break;
        }
    }
    students.insert(students.begin()+new_student_sort_by_name_id, {&new_student, false});
    return new_student_sort_by_name_id;
};

bool Group::delete_student(Student& to_remove_student)
{
    bool is_found_sort_id = false;
    for (int i = 0; i < students.size(); i++)
    {
        if (!is_found_sort_id)
        {
            if (to_remove_student==PTRREF(students[i].student))
            {
                if (students[i].is_deleted) break;
                students[i].is_deleted = true;
                is_found_sort_id = true;
                break;
            }
        }
    }
    return is_found_sort_id;
}

bool Group::is_in_group(Student& student) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (student==PTRREF(students[i].student)) 
        {
            //if (students[i].is_deleted) return false;
            //if (student.is_removed()) return false;
            return true;
        }
    }
    return false;
}

bool Group::is_deleted(Student& student) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (student==PTRREF(students[i].student)) 
        {
            if (students[i].is_deleted) return true;
            //if (student.is_removed()) return true;
            return false;
        }
    }
    return false;
}

bool Group::restore_student(Student& student)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (student==PTRREF(students[i].student)) 
        {
            students[i].is_deleted = false;
            //if (student.is_removed()) return true;
            return true;
        }
    }
    return false;
}

bool Group::set_comment(std::string new_comment)
{
    comment = new_comment;
    return true;
}

std::string Group::get_comment()
{
    return comment;
}

std::string Group::get_description()
{
    std::string output = "#" + std::to_string(get_number());
    if (get_day_of_the_week() != -1) output.append(", " + Day_Names[get_day_of_the_week()]);
    if (comment.size() != 0) output.append(", " + get_comment());
    return output;
}

Group::Group()
{
    group_info.day_of_the_week = -1;
    group_info.number = -1;
}

bool Group::operator==(const Group& rhs) const { return this == &rhs; };

Attend_Data Group::get_attend_data(int known_student_id) const
{
    return students[known_student_id].attend_data;
}
bool Group::set_attend_data(int known_student_id, Attend_Data new_attend_data)
{
    if (new_attend_data > 2 || new_attend_data < 0) return false;
    students[known_student_id].attend_data = new_attend_data;
    return true;
}