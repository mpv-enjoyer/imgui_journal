#pragma once
#include "storage.h"

static std::vector<Student*> _all_students;
static std::vector<Group*> _all_groups;
static std::vector<std::vector<Lesson_Info*>> _all_lessons(7, std::vector<Lesson_Info*>()); //[day_of_the_week][]
static std::vector<Calendar_Day*> _all_days;