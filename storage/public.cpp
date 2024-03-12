#include "public.h"
#include "internal.h"

namespace Journal
{
    const int current_year() { Internal_Storage::_current_year; };
    const int current_month() { return Internal_Storage::_current_month; };
    const int current_day_of_the_week() { Internal_Storage::_current_day_of_the_week; };
    const int current_month_days_num() { return Internal_Storage::_current_month_days_num; };
    const std::vector<Visible_Day>& visible_days() { return std::ref(Internal_Storage::_visible_days); };
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
            Calendar_Day* current_visible = Internal_Storage::_all_days[first_visible_day-MDAY_DIFF];
            Internal_Storage::_visible_days.push_back(Visible_Day{first_visible_day, current_visible, is_future, is_today});
        }
    }
    void add_student_to_base(std::string name, int contract)
    {
        IM_ASSERT(contract >= 0 && name.size() > 0);
        Student* current = new Student();
        current->set_contract(contract);
        current->set_name(name);
        Internal_Storage::_all_students.push_back(current);
    }
    void add_merged_lesson(int day_of_the_week, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs)
    {
        IM_ASSERT(day_of_the_week >= 0 && day_of_the_week <= 6 && number >= 0 && age_group >= 0 && age_group <= AGE_GROUP_COUNT);
        Group* group = new Group();
        group->set_age_group(age_group);
        group->set_comment(comment);
        group->set_number(number);
        Internal_Storage::_all_groups.push_back(group);
        Lesson_Info* current = new Lesson_Info();
        for (int i = 0; i < MAX_INTERNAL_LESSONS; i++)
            current->add_lesson_pair(lesson_pairs[i]);
        current->set_group(PTRREF(group));
        std::vector<Lesson_Info*>& lessons_in_this_day = std::ref(Internal_Storage::_all_lessons[day_of_the_week]);
        int new_merged_lesson_known_id = lessons_in_this_day.size();
        for (int i = 0; i < lessons_in_this_day.size(); i++)
        {
            if (PTRREF(lessons_in_this_day[i]) > PTRREF(current))
            {
                new_merged_lesson_known_id = i;
                break;
            }
        }
        lessons_in_this_day.insert(lessons_in_this_day.begin() + new_merged_lesson_known_id, current);
        for (int i = 0; i < Journal::visible_days().size(); i++)
        {
            bool await_no_one = false;
            if (!(Journal::visible_days()[i].is_future || Journal::visible_days()[i].is_today)) await_no_one = true;
            Journal::visible_days()[i].day->add_merged_lesson(PTRREF(current), await_no_one, new_merged_lesson_known_id);
        }
    }
    void add_student_to_group() //TODO...
}