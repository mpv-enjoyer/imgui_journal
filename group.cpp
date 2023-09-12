#include "main.h"

int Group::get_student_sort_id(int student)
{   
    return students_sort_by_id[student];
}

int Group::get_number()
{
    return group_info.number;
}

bool Group::set_number(int new_number)
{
    group_info.number = new_number;
    return true;
}

int Group::get_size()
{
    return students_sort_by_id.size();
}

int Group::get_day_of_the_week()
{
    return group_info.day_of_the_week;
}

bool Group::set_day_of_the_week(int new_day)
{
    if (new_day < -1 || new_day >= 7) return false;
    group_info.day_of_the_week = new_day;
    return true;
}

int Group::add_student(int new_student_id) //in case name is equal to someone else's: less student_id comes first
//returns his internal id
{
    int new_student_sort_by_id_id = students_sort_by_id.size(); //ascending

    for (int i = 0; i < (students_sort_by_id.size()); i++)
    {
        if (new_student_id == students_sort_by_id[i])
        {
            return -1; //already in the list
        }
        if (new_student_id < students_sort_by_id[i])
        {
            new_student_sort_by_id_id = i; break;
        }
    }
    //use deleted_students_sort_by_id ????????
    students_sort_by_id.insert(students_sort_by_id.begin()+new_student_sort_by_id_id, new_student_id);
    return new_student_sort_by_id_id;
};

bool Group::delete_student(int to_remove_student_id)
{
    bool is_found_sort_id = false;
    for (int i = 0; i < students_sort_by_id.size(); i++)
    {
        if (!is_found_sort_id)
        {
            if (students_sort_by_id[i]==to_remove_student_id)
            {
                deleted_students_sort_by_id.push_back(to_remove_student_id);
                students_sort_by_id.erase(students_sort_by_id.begin()+i);
                is_found_sort_id = true;
            }
        }
    }
    return is_found_sort_id;
}

bool Group::is_in_group(int student)
{
    for (int i = 0; i < students_sort_by_id.size(); i++)
    {
        if (students_sort_by_id[i]==student) return true;
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

Group::Group(std::vector<Student>* students_list)
{
    Group::all_students = students_list;
    group_info.day_of_the_week = -1;
}

