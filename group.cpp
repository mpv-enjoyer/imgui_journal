#include "main.h"

int Group::get_student_sort_id(int student)
{   
    return students_sort_by_id[student];
}

int Group::get_number()
{
    return number;
}

bool Group::set_number(int new_number)
{
    number = new_number;
    return true;
}

int Group::get_size()
{
    return students_sort_by_id.size();
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
                students_sort_by_id.erase(students_sort_by_id.begin()+i);
                is_found_sort_id = true;
            }
        }
    }
    return is_found_sort_id;
}

Group::Group(std::vector<Student>* students_list)
{
    Group::all_students = students_list;
}

bool Group::is_in_group(int student)
{
    for (int i = 0; i < students_sort_by_id.size(); i++)
    {
        if (students_sort_by_id[i]==student) return true;
    }
    return false;
}