#include "lessons_week.h"

Lessons_Day *LessonsWeek::lessons_day(WdayEnglish wday)
{
    return &(_lessons_days[wday.value]);
}
