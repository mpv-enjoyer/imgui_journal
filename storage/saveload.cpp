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

void Journal::save_workouts()
{
    if (!_check_rights({State::Fullaccess, State::Limited, State::Preview})) return;
    std::ofstream ofs(generate_workout_name(Workout_Handler::get_bottom_year(_current_month, _current_year)));
    boost::archive::text_oarchive oa(ofs);
    oa << _workout_handler;
}

void Journal::save()
{
    if (restrict_saving) return;
    save_workouts();
    if (!_check_rights({State::Fullaccess, State::Limited})) return;
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

    std::ifstream ifs(generate_file_name(base_month, base_year));
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

    /* Remove removed lessons for real? */

    // Fill lessons status for workouts depending on other months:
    auto workouts = _workout_handler->search_info(_current_month);
    for (auto workout : workouts)
    {
        int student_id = workout->should_student_id;
        const Student& current_student = PTRREF(student(student_id));
        const Group& group = lesson_info(wday(workout->should_attend.tm_mday), workout->should_lesson.merged_lesson_id)->get_group();
        int internal_student_id = group.find_student(current_student);
        Student_Status status;
        status.discount_status = _discount_status(current_student.get_contract());
        status.status = STATUS_WORKED_OUT;
        set_lesson_status(workout->should_attend.tm_mday, workout->should_lesson, internal_student_id, status, false);
    }
}

void Journal::generate()
{
    _all_days.clear();
    _all_groups.clear();
    _all_lessons.clear();
    _all_students.clear();
    _all_lessons = std::vector<std::vector<Lesson_Info*>>(7);
    int first_day_of_the_week = get_first_mwday(_current_month, _current_year);
    for (int i = 0; i < _current_month_days_num; i++)
    {
        int wday = (first_day_of_the_week + i) % 7;
        _all_days.push_back(new Calendar_Day(_all_lessons[wday]));
    }
}

bool Journal::_check_rights(std::vector<State> states)
{
    for (auto state : states)
    {
        if (state == _state) return true;
    }
    return false;
}

Journal::~Journal()
{
    save();
    for (auto& lesson_info_day : _all_lessons)
    {
        for (auto& lesson_info : lesson_info_day)
        {
            delete lesson_info;
        }
    }
    for (auto& group : _all_groups)
    {
        delete group;
    }
    for (auto& student : _all_students)
    {
        delete student;
    }
    bool is_current_year_allocated = _journal_main_bottom_year != Workout_Handler::get_bottom_year(current_month(), current_year());
    if (is_current_year_allocated) delete _workout_handler;
}
