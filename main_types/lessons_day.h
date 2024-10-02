#pragma once
#include "main_types.h"

class Lessons_Day : public Container<Lesson_Info*>
{
    NON_COPYABLE_NOR_MOVABLE(Lessons_Day);
public:
    Lessons_Day() { };
    const Lesson_Info* lesson_info(ID id) const;
    std::vector<const Lesson_Info*> lesson_infos() const;
    void add_lesson_info(Group::Number number, Group::Comment comment, Group::AgeGroup age_group, std::vector<InternalLessonInfo> lesson_pairs);
    Lessons_Day::ID find_merged_lesson(const Lesson_Info* lesson_info) const;
};

typedef Lessons_Day::ID MergedLessonID;