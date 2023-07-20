#include "main.h"

int Student::get_contract() 
{
    return contract;
}

icu::UnicodeString Student::get_name()
{
    return name;
}

int Student::get_lessons_size()
{
    return lesson_info_ids.size();
}

bool Student::set_contract(int new_contract)
{
    contract = new_contract;
    return true;
}

bool Student::set_name(icu::UnicodeString new_name)
{
    if (new_name.length()==0) return false;
    name = new_name;
    return true;
}

Student::Student()
{

}