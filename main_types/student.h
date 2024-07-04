#pragma once
#include "main_types.h"
#include <boost/container_hash/hash.hpp>

class Student
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & age_group;
        ar & name;
        ar & contract;
        ar & removed;
    }
private:
    bool removed = 0;
    int contract;
    std::string name;
    int age_group = -1;
public:
    Student();
    int get_contract() const; bool set_contract(int new_contract);
    std::string get_name() const; bool set_name(std::string new_name);
    int get_age_group() const; std::string get_age_group_string(); bool set_age_group(int new_age_group);
    bool is_removed() const; bool remove(); bool restore();
    bool is_identical(const Student& rhs) const; //this is an entry check. Once there is an identical student for some reason, do nothing about it.
    bool operator==(const Student& rhs) const;
    bool operator!=(const Student& rhs) const;
    bool operator< (const Student& rhs) const;
    bool operator> (const Student& rhs) const;
    std::size_t operator()() const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, name);
        boost::hash_combine(seed, contract);
        return seed;
    };
};