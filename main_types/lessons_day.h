#pragma once
#include "main_types.h"

class Lessons_Day
{
    NON_COPYABLE_NOR_MOVABLE(Lessons_Day);
    std::vector<Lesson_Info*> _lessons;
public:
    Lessons_Day() { };
    const Lesson_Info* lesson_info(InternalLessonID id) const;
    std::vector<const Lesson_Info*> lesson_infos() const;
    void add_lesson_info(Group::Number number, Group::Comment comment, Group::AgeGroup age_group, std::vector<InternalLessonInfo> lesson_pairs);
};