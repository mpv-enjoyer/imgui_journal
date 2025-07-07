#pragma once
#include "main_types.h"

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
private:
    const std::size_t ID;
    bool removed = 0;
    int contract;
    std::string name;
public:
    Student(std::size_t id);
    int get_contract() const; bool set_contract(int new_contract);
    std::string get_name() const; bool set_name(std::string new_name);
    bool is_removed() const; bool remove(); bool restore();
    std::size_t get_id() const;
    bool is_identical(const Student& rhs) const; //this is an entry check. Once there is an identical student for some reason, do nothing about it.
    bool operator==(const Student& rhs) const;
    bool operator!=(const Student& rhs) const;
    bool operator< (const Student& rhs) const;
    bool operator> (const Student& rhs) const;
};