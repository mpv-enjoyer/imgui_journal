#pragma once
#include "lessons_week.h"
#include "calendar_day.h"

class MonthInfo
{
    struct DayInfo
    {
        WdayEnglish wday;
        Lessons_Day* lessons_day;
        Calendar_Day* calendar_day;
    };
};