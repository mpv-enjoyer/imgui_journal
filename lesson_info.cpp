#include "main.h"

Lesson_Info::Lesson_Info(std::vector<Group>* all_groups)
{
    Lesson_Info::all_groups = all_groups;
}

int Lesson_Info::get_group()
{
    return group;
}

int Lesson_Info::get_day_of_the_week()
{
    return day_of_the_week;
}

Lesson_Pair Lesson_Info::get_lesson_pair(int id)
{
    return lesson_pairs[id];
}

int Lesson_Info::get_lessons_size()
{
    return lesson_pairs.size();
}

bool Lesson_Info::set_group(int new_group_id)
{
    group = new_group_id;
    return true;
}

bool Lesson_Info::set_day_of_the_week(int new_day)
{
    if (new_day >= 7) return false;
    day_of_the_week = new_day;
    return true;
} 

bool Lesson_Info::add_lesson_pair(Lesson_Pair new_lesson_pair)
{
    
}