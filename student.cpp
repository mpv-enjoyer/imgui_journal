#include "student.h"

int Student::get_contract() const
{
    return contract;
}

std::string Student::get_name() const
{
    return name;
}

int Student::get_lessons_size()
{
    return lessons_ignore.size();
}

bool Student::add_lesson_ignore(Lesson_Info& new_lesson, int internal_lesson_id)
{
    lessons_ignore.push_back(Lesson_Ignore { new_lesson, internal_lesson_id });
    return true;
}

bool Student::is_ignored(Lesson_Info& lesson, int internal_lesson) const
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (&lesson == &(lessons_ignore[i].merged_lesson) && internal_lesson == lessons_ignore[i].internal_lesson_id) return true;
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

bool Student::delete_lesson_ignore(Lesson_Info& lesson_to_delete, int internal_lesson)
{
    for (int i = 0; i < lessons_ignore.size(); i++)
    {
        if (&(lessons_ignore[i].merged_lesson) == &lesson_to_delete && lessons_ignore[i].internal_lesson_id == internal_lesson) 
        {
            lessons_ignore.erase(lessons_ignore.begin()+i);
            return true;
        }
    }
    return false;
}

bool Student::is_removed() const
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

bool Student::is_identical(const Student& rhs) const
{
    if (removed) return false;
    if (name == rhs.name && contract == rhs.contract) return true;
    return false;
}

Student::Student() { };

bool Student::operator==(const Student& rhs) const { return this == &rhs; }
bool Student::operator!=(const Student& rhs) const { return !(this == &rhs); }
bool Student::operator< (const Student& rhs) const { return name < rhs.get_name() || (name == rhs.get_name() && contract < rhs.get_contract()); }
bool Student::operator> (const Student& rhs) const { return name > rhs.get_name() || (name == rhs.get_name() && contract > rhs.get_contract()); }