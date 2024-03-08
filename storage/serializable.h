#pragma once
#include "storage.h"

namespace Journal
{
std::vector<Student*> all_students;
    std::vector<Group*> all_groups;
    std::vector<std::vector<Lesson_Info*>> all_lessons(7, std::vector<Lesson_Info*>()); //[day_of_the_week][]
    std::vector<Calendar_Day*> all_days;
}