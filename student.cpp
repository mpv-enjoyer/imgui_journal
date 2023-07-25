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
    return lesson_info_ignore_ids.size();
}

bool Student::add_lesson_ignore_id(int new_lesson_info_id)
{
    lesson_info_ignore_ids.push_back(int new_lesson_info_id);
    return true;
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

Student::Student()
{

}