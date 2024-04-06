#include "journal.h"

std::string generate_file_name(int month, int year)
{
    return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y.data";
}

void Journal::save()
{
    std::ofstream ofs(generate_file_name(_current_month, _current_year));
    boost::archive::text_oarchive oa(ofs);
    oa << _all_students;
    oa << _all_groups;
    oa << _all_lessons;
    oa << _all_days;
}

bool Journal::load()
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

void Journal::generate_current()
{
    _all_days.clear();
    _all_groups.clear();
    _all_lessons.clear();
    _all_students.clear();
    //TODO CRITICAL: Look up for workouts and repair their pointers
    bool surpass_year = _current_month == 0;
    int previous_month = (_current_month + 12 - 1) % 12;
    int previous_year = surpass_year ? _current_year - 1 : _current_year;
    std::ifstream previous_ifs(generate_file_name(previous_month, previous_year));
    int first_day_of_the_week = get_first_mwday(_current_month, _current_year);
    _all_lessons = std::vector<std::vector<Lesson_Info*>>(7);
    for (int i = 0; i < _current_month_days_num; i++)
    {
        int wday = (first_day_of_the_week + i) % 7;
        _all_days.push_back(new Calendar_Day(_all_lessons[wday]));
    }
    if (previous_ifs.fail())
    {
        return;
    }
    throw std::invalid_argument("not implemented");
    //TODO: do not forget to add _days for imported previous Lesson_Info
    //TODO: also looks like I only need to have all_lessons in my inter_month save files
}