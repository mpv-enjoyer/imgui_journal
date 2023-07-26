#include "main.h"

Lesson_Info::Lesson_Info(std::vector<Group>* all_groups)
{
    Lesson_Info::all_groups = all_groups;
}

int Lesson_Info::get_group()
{
    return group;
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

bool Lesson_Info::add_lesson_pair(Lesson_Pair new_lesson_pair)
{
    int new_lesson_pair_id = get_lessons_size();
    if (new_lesson_pair.time_end <= new_lesson_pair.time_begin) return false;
    for (int i = 0; i < get_lessons_size(); i++)
    {
        if (lesson_pairs[i].time_begin > new_lesson_pair.time_begin)
        {
            new_lesson_pair_id = i;
            break;
        }
    }
    lesson_pairs.insert(lesson_pairs.begin()+new_lesson_pair_id, new_lesson_pair);
    return true;
}

bool Lesson_Info::delete_lesson_pair(int id)
{
    if (id >= get_lessons_size()) return false;
    lesson_pairs.erase(lesson_pairs.begin()+id);
    return true;
}