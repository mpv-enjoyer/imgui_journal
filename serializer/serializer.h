#pragma once
#include "../main.h"
#include "../main_types/main_types.h"

class Serializer
{
    std::vector<Student*>& all_students;
    std::vector<Group*>& all_groups;
    std::vector<std::vector<Lesson_Info*>>& all_lessons;
    std::vector<std::vector<Calendar_Day*>>& all_days;
};