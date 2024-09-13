#include "lesson_info.h"

Lesson_Info::Lesson_Info(Group &connected_group, std::vector<InternalLessonInfo> internal_lesson_infos)
: _group(&connected_group), _internal_lesson_infos(internal_lesson_infos)
{

}

const Group &Lesson_Info::get_group() const
{
    return PTRREF(_group);
}

Group& Lesson_Info::get_group()
{
    return PTRREF(_group);
}

InternalLessonInfo Lesson_Info::get_lesson_pair(InternalLessonID id) const
{
    return _internal_lesson_infos.get_lesson_pair(id);
}

int Lesson_Info::get_lessons_size() const
{
    return _internal_lesson_infos.size();
}

std::vector<InternalLessonInfo> Lesson_Info::get_lesson_pairs() const
{
    return _internal_lesson_infos.get_lesson_pairs();
}

std::string Lesson_Info::get_description(int current_internal_lesson) const
{
    std::string to_return = "Группа " + _group->get_description();
    if (current_internal_lesson == -1)
    {
        for (int i = 0; i < get_lessons_size(); i++)
        {
            to_return.append(", ");
            InternalLessonInfo pair = get_lesson_pair(i);
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
        InternalLessonInfo pair = get_lesson_pair(i);
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

void Lesson_Info::edit_lesson_pair(InternalLessonID id, JTime begin, JTime end)
{
    _internal_lesson_infos.edit_pair(id, begin, end);
}

bool Lesson_Info::operator==(const Lesson_Info& rhs) const { return this == &rhs; }
bool Lesson_Info::operator!=(const Lesson_Info& rhs) const { return !(this == &rhs); }
bool Lesson_Info::operator< (const Lesson_Info& rhs) const
{
    JTime lhs_begin = _internal_lesson_infos.get_lesson_pair(0).time_begin;
    JTime rhs_begin = rhs.get_lesson_pair(0).time_begin;
    return std::tie(lhs_begin.hours, lhs_begin.minutes)
         < std::tie(rhs_begin.hours, rhs_begin.minutes);
}
bool Lesson_Info::operator> (const Lesson_Info& rhs) const { return rhs < *this; };