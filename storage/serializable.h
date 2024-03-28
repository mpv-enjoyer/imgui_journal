#pragma once
#include "storage.h"

std::vector<Student*>& _all_students_();
std::vector<Group*>& _all_groups_();
std::vector<std::vector<Lesson_Info*>>& _all_lessons_(); //[day_of_the_week][]
std::vector<Calendar_Day*>& _all_days_();