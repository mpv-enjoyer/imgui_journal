#include "main.h"


int Group::get_id()
{
    return id;
}

std::vector<Student> Group::get_students()
{
    std::vector<Student> group_sort_by_name;
    for (int i = 0; i < name_sort_ids.size(); i++)
    {
        group_sort_by_name.push_back(students[name_sort_ids[i]]);
    }
    return group_sort_by_name;
}

bool Group::add_student(Student new_student) //in case name is equal to someone else's: less student_id comes first
{
    int new_student_vector_id = 0; //ascending
    int new_student_sort_by_name_id = 0; //descending
    std::string new_student_name = new_student.get_name();
    int new_student_id = new_student.get_id();
    for (int i = 0; i < name_sort_ids.size(); i++)
    {
        if (new_student_name.compare(students[i].get_name()) > 0)
    }
};