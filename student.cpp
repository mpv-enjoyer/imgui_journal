#include "main.h"

int Student::get_id() 
{
    return id;
}

int Student::get_contract() 
{
    return contract;
}

icu::UnicodeString Student::get_name()
{
    return name;
}

bool Student::set_contract(int new_contract)
{
    contract = new_contract;
    return true;
}

bool Student::set_name(icu::UnicodeString new_name) //TODO: store lowercase, output with proper casing. Easy with human names.
{
    if (new_name.length()==0) return false;
    name = new_name;
    return true;
}

Student::Student()
{
    student_max_id++;
    id = student_max_id;
}