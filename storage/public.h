#pragma once
#include "storage.h"
#include "internal.h"
#include "serializable.h"

namespace Journal
{
    const int& current_year = std::ref(Internal_Storage::_current_year);
    const int& current_month = std::ref(Internal_Storage::_current_month);
    const int& current_day_of_the_week = std::ref(Internal_Storage::_current_day_of_the_week);
    const int& current_month_days_num = std::ref(Internal_Storage::_current_month_days_num);
    const std::vector<Visible_Day>& visible_days() { return std::ref(Internal_Storage::_visible_days); };
    const std::tm current_time = *std::localtime(&Internal_Storage::_current_timestamp);
    const std::vector<Student*>& all_students() { return std::ref(Internal_Storage::_all_students); };
    const std::vector<Group*>& all_groups() { return std::ref(Internal_Storage::_all_groups); };
    const std::vector<std::vector<Lesson_Info*>>& all_lessons() { return std::ref(Internal_Storage::_all_lessons); };
    const std::vector<Calendar_Day*>& all_days() { return std::ref(Internal_Storage::_all_days); };
    std::vector<Student*>& edit_all_students() { return std::ref(Internal_Storage::_all_students); };
    std::vector<Group*>& edit_all_groups() { return std::ref(Internal_Storage::_all_groups); };
    std::vector<std::vector<Lesson_Info*>>& edit_all_lessons() { return std::ref(Internal_Storage::_all_lessons); };
    std::vector<Calendar_Day*>& edit_all_days() { return std::ref(Internal_Storage::_all_days); };
    void set_date(int month, int year)
    {
        if (month != current_time.tm_mon || year != current_time.tm_year) throw std::invalid_argument("not implemented");
        //TODO CRITICAL: check if journal is available for this month and load.
        Internal_Storage::_current_month = month;
        Internal_Storage::_current_year = year;
        Internal_Storage::_current_month_days_num = get_number_of_days(month, year + 1900);
    }
    void set_day_of_the_week(int day_of_the_week)
    {
        Internal_Storage::_current_day_of_the_week = day_of_the_week;
        int first_visible_day = get_first_wday(Internal_Storage::_current_month, Internal_Storage::_current_year, day_of_the_week);
        Internal_Storage::_visible_days.clear();
        for (;first_visible_day <= Internal_Storage::_current_month_days_num; first_visible_day+=7)
        {
            bool is_future = current_time.tm_mday < first_visible_day;
            bool is_today = current_time.tm_mday == first_visible_day;
            Calendar_Day* current_visible = all_days()[first_visible_day-MDAY_DIFF];
            Internal_Storage::_visible_days.push_back(Visible_Day{first_visible_day, current_visible, is_future, is_today});
        }
    }
}