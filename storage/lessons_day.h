#pragma once
#include "../main_types/main_types.h"

class Lessons_Day
{
    NON_COPYABLE_NOR_MOVABLE(Lessons_Day);
    std::vector<Lesson_Info*> _lessons;
public:
    Lessons_Day() { };
    const Lesson_Info* lesson_info(int id) const;
    std::vector<const Lesson_Info*> lesson_infos() const;
    void add_lesson_info();
};