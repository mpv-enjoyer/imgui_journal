#include "public.h"
#include "internal.h"

namespace Journal
{
    std::string generate_file_name(int month, int year)
    {
        return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y.data";
    }

    void save()
    {
        std::ofstream ofs(generate_file_name(_current_month, _current_year));
        boost::archive::text_oarchive oa(ofs);
        oa << _all_students;
        oa << _all_groups;
        oa << _all_lessons;
        oa << _all_days;
    }

    bool load()
    {
        std::ifstream ifs(generate_file_name(_current_month, _current_year));
        if (ifs.fail()) return false;
        boost::archive::text_iarchive ia(ifs);
        ia >> _all_students;
        ia >> _all_groups;
        ia >> _all_lessons;
        ia >> _all_days;
        return true;
    }

    void generate_current()
    {
        std::vector<Student*> all_students;
        std::vector<Group*> all_groups;
        std::vector<std::vector<Lesson_Info*>> all_lessons(7, std::vector<Lesson_Info*>()); //[day_of_the_week][]
        std::vector<Calendar_Day*> all_days;
        
        //TODO CRITICAL: Look up for workouts and repair their pointers
        bool surpass_year = _current_month == 0;
        int previous_month = (_current_month + 12 - 1) % 12;
        int previous_year = surpass_year ? _current_year - 1 : _current_year;
        std::ifstream previous_ifs(generate_file_name(previous_month, previous_year));

        int first_day_of_the_week = calculate_first_mwday(_current_month, _current_year);
        for (int i = 0; i < _current_month_days_num; i++)
        {
           all_days.push_back(new Calendar_Day(all_lessons[(first_day_of_the_week + i) % 7]));
        }

        if (previous_ifs.fail())
        {
            return;
        }
        //TODO: do not forget to add _days for imported previous Lesson_Info
        //TODO: also looks like I only need to have all_lessons in my inter_month save files
    }
}