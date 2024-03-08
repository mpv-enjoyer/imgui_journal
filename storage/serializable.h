#pragma once
#include "storage.h"

namespace Internal_Storage
{
    std::vector<Student*> _all_students;
    std::vector<Group*> _all_groups;
    std::vector<std::vector<Lesson_Info*>> _all_lessons(7, std::vector<Lesson_Info*>()); //[day_of_the_week][]
    std::vector<Calendar_Day*> _all_days;
}