#pragma once
#include "main.h"
#include "group.h"

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    bool removed = 0;
    Group& group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info(Group& connected_group);
    const Group& get_group() const; bool Lesson_Info::set_group(Group& new_group);
    Lesson_Pair get_lesson_pair(int id) const; bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    bool discontinue(); bool is_discontinued();
    bool Lesson_Info::should_attend(Student& student) const;
    int get_lessons_size() const;
    std::string get_description(int current_internal_lesson = -1) const;
    bool operator==(const Lesson_Info& rhs) { return this == &rhs; }
    bool operator!=(const Lesson_Info& rhs) { return !(this == &rhs); }
    bool operator< (const Lesson_Info& rhs) 
    {
        if (lesson_pairs[0].time_begin < rhs.get_lesson_pair(0).time_begin) return true;
        if (lesson_pairs[0].time_begin > rhs.get_lesson_pair(0).time_begin) return false;
        return group.get_number() < rhs.get_group().get_number();
    }
    bool operator> (const Lesson_Info& rhs) { return !(*this < rhs) && (*this != rhs); };
};