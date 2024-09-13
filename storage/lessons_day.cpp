#include "lessons_day.h"

const Lesson_Info *Lessons_Day::lesson_info(int id) const
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

void Lessons_Day::add_lesson_info()
{
    Group* group = new Group();
    group->set_age_group(age_group);
    group->set_comment(comment);
    group->set_number(number);
    _all_groups.push_back(group);
    Lesson_Info* current = new Lesson_Info();
    for (int i = 0; i < lesson_pairs.size(); i++)
        current->add_lesson_pair(lesson_pairs[i]);
    current->set_group(PTRREF(group));
}
