#pragma once
#include "main_types.h"
#include <boost/container_hash/hash.hpp>

class Student
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & contract;
        ar & removed;
    }
public:
    NO_IMPLICIT_CONVERSION_T(int, Contract);
    NO_IMPLICIT_CONVERSION_T(std::string, Name);
    NON_COPYABLE_NOR_MOVABLE(Student);
private:
    Student() { };
    bool removed = 0;
    Contract _contract = 0;
    Name _name = std::string();
public:
    Student(Contract contract, Name name);
    Student::Contract get_contract() const;
    Student::Name get_name() const;
    bool set_contract(Contract contract);
    bool set_name(Name new_name);
    bool is_removed() const; bool remove(); bool restore();
    friend bool operator==(const Student& lhs, const Student& rhs);
    friend bool operator!=(const Student& lhs, const Student& rhs);
    friend bool operator< (const Student& lhs, const Student& rhs);
    friend bool operator> (const Student& lhs, const Student& rhs);
};