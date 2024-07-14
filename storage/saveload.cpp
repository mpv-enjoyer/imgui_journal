#include "journal.h"

std::string generate_file_name(int month, int year)
{
    return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y.data";
}

std::string generate_workout_name(int bottom_year)
{
    return "save_" + std::to_string(bottom_year + 1900) + "y_workouts.data";
}

Journal::State Journal::get_state()
{
    return _state;
}

bool Journal::save_file_exists(int month, int year)
{
    std::ifstream ifs(generate_file_name(month, year));
    return ifs.good();
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

bool Journal::_load_workouts()
{
    std::ifstream workout_ifs(generate_workout_name(Workout_Handler::get_bottom_year(_current_month, _current_year)));
    if (workout_ifs.fail())
    {
        return false;
    }
    boost::archive::text_iarchive workout_ia(workout_ifs);
    workout_ia >> _workout_handler;
    return true;
}

bool Journal::load()
{
    _all_days.clear();
    _all_groups.clear();
    _all_lessons.clear();
    _all_students.clear();
    std::ifstream ifs(generate_file_name(_current_month, _current_year));
    if (ifs.fail()) 
    {
        return false;
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> _all_students;
    ia >> _all_groups;
    ia >> _all_lessons;
    ia >> _all_days;

    if (!_load_workouts()) IM_ASSERT(false && "Loaded month must load workouts...");
    return true;
}

void Journal::generate(int base_month, int base_year)
{
    _all_days.clear();
    _all_groups.clear();
    _all_lessons.clear();
    _all_students.clear();

    std::ifstream ifs(generate_file_name(_current_month, _current_year));
    IM_ASSERT(ifs.good());
    boost::archive::text_iarchive ia(ifs);
    ia >> _all_students;
    ia >> _all_groups;
    ia >> _all_lessons;
    int first_day_of_the_week = get_first_mwday(_current_month, _current_year);
    for (int i = 0; i < _current_month_days_num; i++)
    {
        int wday = (first_day_of_the_week + i) % 7;
        _all_days.push_back(new Calendar_Day(_all_lessons[wday]));
    }
    // Remove removed lessons for real?
}

void Journal::generate()
{
    //_generated = true;
    _all_days.clear();
    _all_groups.clear();
    _all_lessons.clear();
    _all_students.clear();
    int first_day_of_the_week = get_first_mwday(_current_month, _current_year);
    for (int i = 0; i < _current_month_days_num; i++)
    {
        int wday = (first_day_of_the_week + i) % 7;
        _all_days.push_back(new Calendar_Day(_all_lessons[wday]));
    }
}

//Limited access should be given to other months to prevent weird behaviour.
//Limited access will allow to:
// set status
// set/unset workouts
bool Journal::is_full_access()
{
    return _current_month == current_time.tm_mon && _current_year == current_time.tm_year;
}
