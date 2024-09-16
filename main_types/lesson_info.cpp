#include "lesson_info.h"

void Lesson_Info::set(InternalLessonInfo data, Container<InternalLessonInfo>::ID id)
{
    IM_ASSERT(id < size());
    _data[id].time_begin = data.time_begin;
    _data[id].time_end = data.time_end;
}

Lesson_Info::Lesson_Info(Group &connected_group, std::vector<InternalLessonInfo> internal_lesson_infos)
    : _group(&connected_group)
{
    for (std::size_t i = 0; i < internal_lesson_infos.size(); i++)
    {
        _data.push_back(internal_lesson_infos[i]);
    }
}

const Group &Lesson_Info::get_group() const
{
    return PTRREF(_group);
}

Group& Lesson_Info::get_group()
{
    return PTRREF(_group);
}

std::string Lesson_Info::get_description(ID id) const
{
    std::string to_return = "Группа " + _group->get_description();
    to_return.append(", ");
    InternalLessonInfo pair = get(id);
    to_return.append(Lesson_Names[pair.lesson_name_id] + " ");
    to_return.append(std::to_string(pair.time_begin.hours) + ":");
    if (pair.time_begin.minutes < 10) to_return.append("0");
    to_return.append(std::to_string(pair.time_begin.minutes));
    to_return.append(" - ");
    to_return.append(std::to_string(pair.time_end.hours) + ":");
    if (pair.time_end.minutes < 10) to_return.append("0");
    to_return.append(std::to_string(pair.time_end.minutes));
    return to_return;
}

std::string Lesson_Info::get_description() const
{
    std::string to_return = "Группа " + _group->get_description();
    for (std::size_t i = 0; i < size(); i++)
    {
        to_return.append(", ");
        InternalLessonInfo pair = get(i);
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

void Lesson_Info::edit_lesson_pair(ID id, JTime begin, JTime end)
{
    InternalLessonInfo info = get(id);
    info.time_begin = begin;
    info.time_end = end;
    set(info, id);
}

//bool Lesson_Info::operator==(const Lesson_Info& rhs) const { return this == &rhs; }
//bool Lesson_Info::operator!=(const Lesson_Info& rhs) const { return !(this == &rhs); }
bool Lesson_Info::operator< (const Lesson_Info& rhs) const
{
    JTime lhs_begin = get(0).time_begin;
    JTime rhs_begin = rhs.get(0).time_begin;
    return std::tie(lhs_begin.hours, lhs_begin.minutes)
         < std::tie(rhs_begin.hours, rhs_begin.minutes);
}
bool Lesson_Info::operator> (const Lesson_Info& rhs) const { return rhs < *this; };