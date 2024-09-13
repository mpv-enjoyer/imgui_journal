#include "student.h"

Student::Contract Student::get_contract() const
{
    return _contract;
}

Student::Name Student::get_name() const
{
    return _name;
}

bool Student::set_contract(Contract contract)
{
    _contract = contract;
    return true;
}

bool Student::set_name(Name name)
{
    // Allow empty names
    _name = name;
    return true;
}

Student::Student(Contract contract, Name name) : _contract(contract), _name(name) { }

bool operator==(const Student &lhs, const Student &rhs)
{
    return std::tie(lhs._name, lhs._contract) == std::tie(rhs._name, rhs._contract);
}

bool operator!=(const Student &lhs, const Student &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Student &lhs, const Student &rhs)
{
    return std::tie(lhs._name, lhs._contract) < std::tie(rhs._name, rhs._contract);
}

bool operator>(const Student &lhs, const Student &rhs)
{
    return rhs < lhs;
}
