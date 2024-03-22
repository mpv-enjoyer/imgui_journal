#include "group.h"

const Student& Group::get_student(int known_student_id) const
{
    return PTRREF(students[known_student_id].student);
}

int Group::get_number() const
{
    return number;
}

bool Group::set_number(int new_number)
{
    number = new_number;
    return true;
}

int Group::get_size() const
{
    return students.size();
}

int Group::get_age_group() const
{
    return age_group;
}

bool Group::set_age_group(int new_day)
{
    if (new_day < -1 || new_day >= AGE_GROUP_COUNT) return false;
    age_group = new_day;
    return true;
}

bool Group::check_with_attend_data(int known_student_id, int internal_lesson) const
{
    Attend_Data attend_data = students[known_student_id].attend_data;
    if (students[known_student_id].is_deleted) return false;
    if (attend_data == ATTEND_FIRST && internal_lesson == 1) return false;
    if (attend_data == ATTEND_SECOND && internal_lesson == 0) return false;
    return true; 
}

bool Group::check_no_attend_data(const Student& student) const
{
    int found = -1;
    for (int i = 0; i < students.size(); i++)
    {
        if (PTRREF(students[i].student) == student)
        {
            found = i;
            break;
        }
    }
    if (found == -1) return false;
    if (students[found].is_deleted) return false;
    return true;
}

int Group::find_student(const Student& student) const
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

bool Group::is_in_group(const Student& student) const
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

bool Group::is_deleted(const Student& student) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (student==PTRREF(students[i].student)) 
        {
            if (students[i].is_deleted) return true;
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

std::string Group::get_comment() const
{
    return comment;
}

std::string Group::get_description()
{
    std::string output = "#" + std::to_string(get_number());
    if (age_group != -1) output.append(", " + Age_Group_Names[age_group]);
    if (comment.size() != 0) output.append(", " + get_comment());
    return output;
}

Group::Group()
{
    age_group = -1;
    number = -1;
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