#include "main.h"

int Student::get_contract() 
{
    return contract;
}

std::string Student::get_name()
{
    return name;
}

int Student::get_lessons_size()
{
    return lessons_ignore.size();
}

bool Student::add_lesson_ignore_id(Lesson new_lesson)
{
    lessons_ignore.push_back(new_lesson);
    return true;
}

bool Student::is_ignored(Lesson lesson)
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (lessons_ignore[i]==lesson) return true;
    }
    return false;
}

bool Student::set_contract(int new_contract)
{
    contract = new_contract;
    return true;
}

bool Student::set_name(std::string new_name)
{
    if (new_name.length()==0) return false;
    name = new_name;
    return true;
}

bool Student::delete_lesson_ignore(Lesson to_delete)
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (lessons_ignore[i]==to_delete) 
        {
            lessons_ignore.erase(lessons_ignore.begin()+i);
            return true;
        }
    }
    return false;
}

bool Student::is_removed()
{
    return removed;
}

bool Student::remove()
{
    removed = true;
    return true;
}

Student::Student()
{

}