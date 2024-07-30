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

bool Student::restore()
{
    removed = false;
    return true;
}

Student::Student()
{
    contract = -1;
}

bool Student::is_identical(const Student& rhs) const
{
    if (removed) return false;
    if (name == rhs.name && contract == rhs.contract) return true;
    return false;
}

bool Student::operator==(const Student& rhs) const { return this == &rhs; }
bool Student::operator!=(const Student& rhs) const { return !(this == &rhs); }
bool Student::operator< (const Student& rhs) const { return contract < rhs.get_contract() || (contract == rhs.get_contract() && name < rhs.get_name()); }
bool Student::operator> (const Student& rhs) const { return contract > rhs.get_contract() || (contract == rhs.get_contract() && name > rhs.get_name()); }