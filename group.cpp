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

int Group::get_number()
{
    return number;
}

bool Group::set_number(int new_number)
{
    Group::number = new_number;
    return true;
}

bool Group::add_student(Student new_student) //in case name is equal to someone else's: less student_id comes first
{
    int new_student_vector_id = 0; //ascending
    int new_student_sort_by_name_id = 0; //ascending
    icu::UnicodeString new_student_name = new_student.get_name();
    int new_student_id = new_student.get_id();
    for (int i = 0; i < name_sort_ids.size(); i++)
    {
        if (new_student_name.compare(students[i].get_name()) > 0 || (new_student_name.compare(students[i].get_name()) == 0 && new_student_id > students[i].get_id()))
        {
            new_student_sort_by_name_id++;
        }
        if (new_student_id > students[i].get_id())
        {
            new_student_vector_id++;
        }
    }

    for (int i = 0; i < name_sort_ids.size(); i++)
    {
        if (name_sort_ids[i]>=new_student_vector_id) 
        {
            name_sort_ids[i] = name_sort_ids[i]+1;
        }
    }
    std::vector<Student>::iterator iterator_students = students.begin();
    std::vector<int>::iterator iterator_name_sort_ids = name_sort_ids.begin();

    students.insert(iterator_students+new_student_vector_id, new_student);
    name_sort_ids.insert(iterator_name_sort_ids+new_student_vector_id, new_student_sort_by_name_id);
    return true;
};

bool Group::delete_student(int to_remove_student)
{
    if (to_remove_student >= students.size()) return false;
    int to_remove_vector_id = name_sort_ids[to_remove_student];
    students.erase(students.begin()+to_remove_vector_id);
    name_sort_ids.erase(name_sort_ids.begin()+to_remove_student);
    for (int i = 0; i < name_sort_ids.size(); i++)
    {
        if (name_sort_ids[i]>to_remove_vector_id) 
        {
            name_sort_ids[i] = name_sort_ids[i] - 1;
        }        
    }
}

Group::Group()
{
    group_max_id++;
    id = group_max_id;
}