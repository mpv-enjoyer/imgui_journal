#include "student.h"

int Student::get_contract() const
{
    return contract;
}

std::string Student::get_name() const
{
    return name;
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

bool Student::operator==(const Student& rhs) const { return this == &rhs; }
bool Student::operator!=(const Student& rhs) const { return !(this == &rhs); }
bool Student::operator< (const Student& rhs) const { return name < rhs.get_name() || (name == rhs.get_name() && contract < rhs.get_contract()); }
bool Student::operator> (const Student& rhs) const { return name > rhs.get_name() || (name == rhs.get_name() && contract > rhs.get_contract()); }