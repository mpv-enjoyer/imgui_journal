#include "journal.h"

std::vector<_Day_With_Info> Journal::_enumerate_days(int day_of_the_week)
{
    std::vector<_Day_With_Info> output;
    int day = get_first_wday(_current_month, _current_year, day_of_the_week);
    int day_count = get_number_of_days(_current_month, _current_year);
    std::tm temp;
    temp.tm_mon = _current_month;
    temp.tm_year = _current_year;
    for ( ; day <= day_count; day+=7)
    {
        temp.tm_mday = day;
        _Day_With_Info current;
        current.day = _day(day - MDAY_DIFF);
        current.number = day;
        current.is_future = current_time < temp;
        current.is_today = current_time == temp;
        output.push_back(current);
    }
    return output;
}

Calendar_Day* Journal::_day(int mday)
{
    return _all_days[mday];
}

int Journal::_discount_status(int student_contract)
{
    int lessons_contract_counter = -1;
    for (const auto& lessons : _all_lessons)
    {
        for (const auto& lesson : lessons)
        {
            const Group& group = lesson->get_group();
            for (int student_id = 0; student_id < group.get_size(); student_id++)
            {
                const Student& student = group.get_student(student_id);
                if (group.is_deleted(student)) continue;
                if (student.is_removed()) continue;
                if (group.get_student(student_id).get_contract() == student_contract) lessons_contract_counter++;
            }
        }
    }
    if (lessons_contract_counter == -1) lessons_contract_counter = 0; // Called by the deleted student?

    int discounts_size = _lesson_prices[0].size();
    if (lessons_contract_counter >= discounts_size) lessons_contract_counter = discounts_size - 1;
    return lessons_contract_counter;
}

int Journal::_emplace_lesson_info(int wday, Lesson_Info &lesson_info)
{
    auto& lessons_in_this_day = _all_lessons[wday];
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
