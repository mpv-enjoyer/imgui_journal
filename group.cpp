#include "main.h"

int Group::get_student_sort_id(int student)
{   
    return students_sort_by_id[student];
}

int Group::get_student_sort_name(int student)
{   
    return students_sort_by_name[student];
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

bool Group::add_student(int new_student_id) //in case name is equal to someone else's: less student_id comes first
{
    int new_student_sort_by_id_id = all_students->size(); //ascending
    int new_student_sort_by_name_id = all_students->size(); //ascending

    for (int i = 0; i < (all_students->size()); i++)
    {
        if (new_student_id == students_sort_by_id[i])
        {
            return false; //already in the list
        }
        if (new_student_id < students_sort_by_id[i])
        {
            new_student_sort_by_id_id = i; break;
        }
    }

    icu::UnicodeString new_student_name = (all_students->at(new_student_id)).get_name();
    for (int i = 0; i < (all_students->size()); i++)
    {
        if (new_student_name.toLower() == (all_students->at(students_sort_by_name[i])).get_name().toLower() && new_student_id < students_sort_by_name[i]);
        {
            new_student_sort_by_name_id = i; break;
        }
        if (new_student_name.toLower() < (all_students->at(students_sort_by_name[i])).get_name().toLower())
        {
            new_student_sort_by_name_id = i; break;
        }
    }
    students_sort_by_id.insert(students_sort_by_id.begin()+new_student_sort_by_id_id, new_student_id);
    students_sort_by_name.insert(students_sort_by_name.begin()+new_student_sort_by_name_id, new_student_id);
    return true;
};

bool Group::delete_student(int to_remove_student_id)
{
    bool is_found_sort_id = false;
    bool is_found_sort_name = false;
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
        if (!is_found_sort_name)
        {
            if (students_sort_by_name[i]==to_remove_student_id)
            {
                students_sort_by_name.erase(students_sort_by_name.begin()+i);
                is_found_sort_name = true;
            }
        }
    }
    return is_found_sort_id && is_found_sort_name;
}

Group::Group(std::vector<Student>* students_list)
{
    Group::all_students = students_list;
}