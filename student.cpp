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

bool Student::add_lesson_ignore_id(Lesson new_lesson, int new_lesson_day_of_the_week)
{
    lessons_ignore.push_back(Lesson_Full{new_lesson, new_lesson_day_of_the_week});
    return true;
}

bool Student::is_ignored(Lesson lesson, int lesson_day_of_the_week)
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (lessons_ignore[i].day_of_the_week == lesson_day_of_the_week && lessons_ignore[i].lesson == lesson) return true;
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

bool Student::delete_lesson_ignore(Lesson lesson_to_delete, int day_of_the_week)
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (lessons_ignore[i].day_of_the_week == day_of_the_week && lessons_ignore[i].lesson == lesson_to_delete) 
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

bool Student::set_age_group(int new_age_group)
{
    if (new_age_group >= AGE_GROUP_COUNT) return false;
    age_group = new_age_group;
    return true;
}

std::string Student::get_age_group_string()
{
    if (age_group == -1) return "не задана";
    return Age_Group_Names[age_group];
}

bool Student::restore()
{
    removed = false;
    return true;
}

int Student::get_age_group()
{
    return age_group;
}

Student::Student()
{
    contract = -1;
    age_group = -1;
}