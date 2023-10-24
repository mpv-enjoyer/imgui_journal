#pragma once
#include "main.h"
#include "helper_types.h"

class Student
{
private:
    bool removed = 0;
    int contract;
    std::string name;
    int age_group = -1;
    std::vector<Lesson_Ignore> lessons_ignore;
public:
    Student();
    int get_contract() const; bool set_contract(int new_contract);
    std::string get_name() const; bool set_name(std::string new_name);
    int get_age_group(); std::string get_age_group_string(); bool set_age_group(int new_age_group);
    bool is_ignored(Lesson_Info& lesson, int lesson_day_of_the_week) const;
    bool add_lesson_ignore(Lesson_Info& new_lesson, int internal_lesson);
    bool delete_lesson_ignore(Lesson_Info& lesson_to_delete, int internal_lesson); 
    int get_lessons_size();
    bool is_removed() const; bool remove(); bool restore();
    bool is_identical(const Student& rhs) const; //this is an entry check. Once there is an identical student for some reason, do nothing about it.
    bool operator==(const Student& rhs) const;
    bool operator!=(const Student& rhs) const;
    bool operator< (const Student& rhs) const;
    bool operator> (const Student& rhs) const;
};