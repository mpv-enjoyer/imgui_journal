#include "lessons_day.h"

const Lesson_Info *Lessons_Day::lesson_info(InternalLessonID id) const
{
    IM_ASSERT(id < _lessons.size());
    return _lessons[id];
}

std::vector<const Lesson_Info *> Lessons_Day::lesson_infos() const
{
    std::vector<const Lesson_Info *> output;
    for (int i = 0; i < _lessons.size(); i++)
    {
        output.push_back(_lessons[i]);
    }
    return output;
}

void Lessons_Day::add_lesson_info(Group::Number number, Group::Comment comment, Group::AgeGroup age_group, std::vector<InternalLessonInfo> lesson_pairs)
{
    Group* group = new Group(number, age_group);
    group->set_comment(comment);
    _lessons.push_back(new Lesson_Info(*group, lesson_pairs));
}
