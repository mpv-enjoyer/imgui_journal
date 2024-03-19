#include "internal.h"

std::vector<Day_With_Info> _enumerate_days(int day_of_the_week)
{
    std::vector<Day_With_Info> output;
    int day = get_first_wday(_current_month, _current_year, day_of_the_week);
    int day_count = get_number_of_days(_current_month, _current_year);
    for ( ; day <= day_count; day+=7)
    {
        Day_With_Info current;
        current.day = _day(day);
        current.number = day;
        current.is_future = day > _current_time.tm_mday;
        current.is_today = day == _current_time.tm_mday;
        output.push_back(current);
    }
    return output;
}

Calendar_Day* _day(int mday)
{
    return _all_days[mday - MDAY_DIFF];
}

int _discount_status(int student_contract)
{
    int output = 0;
    for (auto student : _all_students)
    {
        if (output == LESSON_PRICES_COUNT) return output;
        if (student->get_contract() == student_contract && !student->is_removed())
            output++;
    }
    return output;
}

int _emplace_lesson_info(int wday, Lesson_Info &lesson_info)
{
    auto lessons_in_this_day = _all_lessons[wday];
    int new_merged_lesson_known_id = lessons_in_this_day.size();
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        if (PTRREF(lessons_in_this_day[i]) > lesson_info)
        {
            new_merged_lesson_known_id = i;
            break;
        }
    }
    lessons_in_this_day.insert(lessons_in_this_day.begin() + new_merged_lesson_known_id, &lesson_info);
    return new_merged_lesson_known_id;
}
