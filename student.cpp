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

bool Student::set_birth_date(int year, int month, int day)
{
    if (year <= 1900) return false;
    if (month >= 12 || month < 0) return false;
    if (day >= 32 || day <= 0) return false;
    birth_date.tm_year = year;
    birth_date.tm_mon = month;
    birth_date.tm_mday = day;
    return true;
}

std::string Student::get_birth_date_string()
{
    if (birth_date.tm_mday == -1 || birth_date.tm_mon == -1 || birth_date.tm_year == -1) return "не задана";
    return std::to_string(birth_date.tm_mday) + " " + Month_Names[birth_date.tm_mon] + " " + std::to_string(birth_date.tm_year);
}

Student::Student()
{
    contract = -1;
    birth_date.tm_year = -1;
    birth_date.tm_mon = -1;
    birth_date.tm_mday = -1;
}