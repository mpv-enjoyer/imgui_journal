#pragma once
#include "main.h"

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    bool removed = 0;
    std::vector<Group>* all_groups;
    int group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info(std::vector<Group>* all_groups);
    int get_group() const; bool set_group(int new_group_id);
    Lesson_Pair get_lesson_pair(int id) const; bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    bool remove();
    bool should_attend(int student) const;
    int get_lessons_size() const;
    std::string get_description(int current_internal_lesson = -1) const;
    bool operator==(const Lesson_Info& rhs) { return this == &rhs; }
    bool operator!=(const Lesson_Info& rhs) { return !(this == &rhs); }
    bool operator< (const Lesson_Info& rhs) 
    {
        if (lesson_pairs[0].time_begin < rhs.get_lesson_pair(0).time_begin) return true;
        if (lesson_pairs[0].time_begin > rhs.get_lesson_pair(0).time_begin) return false;
        return group < rhs.get_group();
    }
    bool operator> (const Lesson_Info& rhs) { return !(*this < rhs) && (*this != rhs); };
};