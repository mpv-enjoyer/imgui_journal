#include "lessons_day.h"

const Lesson_Info* Lessons_Day::lesson_info(ID id) const
{
    return get(id);
}

std::vector<const Lesson_Info*> Lessons_Day::lesson_infos() const
{
    std::vector<const Lesson_Info*> output;
    for (std::size_t i = 0; i < size(); i++)
    {
        output.push_back(get(i));
    }
    return output;
}

void Lessons_Day::add_lesson_info(Group::Number number, Group::Comment comment, Group::AgeGroup age_group, std::vector<InternalLessonInfo> lesson_pairs)
{
    Group* group = new Group(number, age_group);
    group->set_comment(comment);
    add(new Lesson_Info(*group, lesson_pairs));
}

MergedLessonID Lessons_Day::find_merged_lesson(const Lesson_Info* lesson_info) const
{
    for (MergedLessonID i = 0; i.value < size(); i.value++)
    {
        if (get(i) == lesson_info) return i;
    }
    return -1;
}