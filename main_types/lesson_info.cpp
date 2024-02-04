#include "lesson_info.h"

Lesson_Info::Lesson_Info(Group& connected_group) : group(&connected_group) {};

Group& Lesson_Info::get_group() const
{
    return PTRREF(group);
}

Lesson_Pair Lesson_Info::get_lesson_pair(int id) const
{
    return lesson_pairs[id];
}

int Lesson_Info::get_lessons_size() const
{
    return lesson_pairs.size();
}

bool Lesson_Info::set_group(Group& new_group)
{
    *group = new_group;
    return true;
}

//Changed behaviour: no more sort check
bool Lesson_Info::add_lesson_pair(Lesson_Pair new_lesson_pair)
{
    int new_lesson_pair_id = get_lessons_size();
    if (new_lesson_pair.time_end <= new_lesson_pair.time_begin) return false;
    lesson_pairs.push_back(new_lesson_pair);
    return true;
}

bool Lesson_Info::delete_lesson_pair(int id)
{
    if (id >= get_lessons_size()) return false;
    lesson_pairs.erase(lesson_pairs.begin()+id);
    return true;
}

bool Lesson_Info::should_attend(Student& student) const
{
    return group->is_in_group(student) && !group->is_deleted(student);
}

std::string Lesson_Info::get_description(int current_internal_lesson) const
{
    std::string to_return = "Группа " + group->get_description();
    if (current_internal_lesson == -1)
    {
        for (int i = 0; i < get_lessons_size(); i++)
        {
            to_return.append(", ");
            Lesson_Pair pair = get_lesson_pair(i);
            to_return.append(Lesson_Names[pair.lesson_name_id] + " ");
            to_return.append(std::to_string(pair.time_begin.hours) + ":");
            if (pair.time_begin.minutes < 10) to_return.append("0");
            to_return.append(std::to_string(pair.time_begin.minutes));
            to_return.append(" - ");
            to_return.append(std::to_string(pair.time_end.hours) + ":");
            if (pair.time_end.minutes < 10) to_return.append("0");
            to_return.append(std::to_string(pair.time_end.minutes));
        }
    }
    else
    {
        int i = current_internal_lesson;
        to_return.append(", ");
        Lesson_Pair pair = get_lesson_pair(i);
        to_return.append(Lesson_Names[pair.lesson_name_id] + " ");
        to_return.append(std::to_string(pair.time_begin.hours) + ":");
        if (pair.time_begin.minutes < 10) to_return.append("0");
        to_return.append(std::to_string(pair.time_begin.minutes));
        to_return.append(" - ");
        to_return.append(std::to_string(pair.time_end.hours) + ":");
        if (pair.time_end.minutes < 10) to_return.append("0");
        to_return.append(std::to_string(pair.time_end.minutes));
    }

    return to_return;
}

bool Lesson_Info::discontinue()
{
    removed = true;
    return true;
}

bool Lesson_Info::is_discontinued() 
{
    return removed;
}

bool Lesson_Info::restore()
{
    removed = false;
    return true;
}

bool Lesson_Info::operator==(const Lesson_Info& rhs) const { return this == &rhs; }
bool Lesson_Info::operator!=(const Lesson_Info& rhs) const { return !(this == &rhs); }
bool Lesson_Info::operator< (const Lesson_Info& rhs) const
{
    if (lesson_pairs[0].time_begin < rhs.get_lesson_pair(0).time_begin) return true;
    if (lesson_pairs[0].time_begin > rhs.get_lesson_pair(0).time_begin) return false;
    return group->get_number() < rhs.get_group().get_number();
}
bool Lesson_Info::operator> (const Lesson_Info& rhs) const { return !(*this < rhs) && (*this != rhs); };