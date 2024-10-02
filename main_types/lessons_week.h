#pragma once
#include "main_types.h"

class LessonsWeek
{
    std::array<Lessons_Day, 7> _lessons_days;
public:
    LessonsWeek() { };
    Lessons_Day* lessons_day(WdayEnglish wday);
};