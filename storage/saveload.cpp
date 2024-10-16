#include "journal.h"
#include <filesystem>

std::string generate_file_name(int month, int year)
{
    return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y.data";
}

std::string generate_workout_name(int bottom_year)
{
    return "save_" + std::to_string(bottom_year + 1900) + "y_workouts.data";
}

std::string generate_prices_name(int month, int year)
{
    return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y_prices.data";
}

std::string generate_prices_name()
{
    return "prices_default.data";
}

bool Journal::get_default_prices(std::vector<std::vector<int>> &prices, int &ill_price, int &skipped_price)
{
    std::ifstream ifs(generate_prices_name());
    if (ifs.fail()) return false;
    boost::archive::text_iarchive ia(ifs);
    ia >> prices;
    ia >> skipped_price;
    ia >> ill_price;
    return true;
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

void Journal::save_backup()
{
    std::string backups_root = "backup";
    std::string current_folder_name = backups_root + "/" + std::to_string(current_time.tm_mday) + "." + std::to_string(current_time.tm_mon + 1) + "." + std::to_string(current_year() + 1900) + "backup";
    if (!std::filesystem::exists(current_folder_name)) std::filesystem::create_directory(current_folder_name);
    std::string workout_path = current_folder_name + "/" + generate_workout_name(_journal_main_bottom_year);
    std::string journal_path = current_folder_name + "/" + generate_file_name(_current_month, _current_year);
    if (!std::filesystem::exists(workout_path)) std::filesystem::copy_file(generate_workout_name(_journal_main_bottom_year), workout_path);
    if (!std::filesystem::exists(journal_path)) std::filesystem::copy_file(generate_file_name(_current_month, _current_year), journal_path);
}

bool Journal::save()
{
    if (restrict_saving) return false;
    save_workouts();
    if (!_check_rights({State::Fullaccess, State::Limited})) return false;

    {
        std::ofstream ofs(generate_file_name(_current_month, _current_year));
        if (ofs.fail()) return false;
        boost::archive::text_oarchive oa(ofs);
        oa << _all_students;
        oa << _all_groups;
        oa << _all_lessons;
        oa << _all_days;
    }

    save_prices();
    try
    {
        save_backup();
    }
    catch(const std::exception& e)
    {
        printf(e.what());
    }
    return true;
}

void Journal::set_default_prices(std::vector<std::vector<int>> prices, int ill_price, int skipped_price)
{
    for (int i = 1; i < prices.size(); i++)
    {
        IM_ASSERT(prices[0].size() == prices[i].size());
    }
    std::ofstream ofs_default(generate_prices_name());
    boost::archive::text_oarchive oa_default(ofs_default);
    oa_default << prices;
    oa_default << skipped_price;
    oa_default << ill_price;
}

void Journal::save_prices()
{
    if (!_check_rights({State::Fullaccess})) return;
    std::ofstream ofs(generate_prices_name(_current_month, _current_year));
    boost::archive::text_oarchive oa(ofs);
    oa << _lesson_prices;
    oa << _lesson_price_skipped;
    oa << _lesson_price_was_ill;
}

bool Journal::load_workouts()
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

    if (!load_workouts()) IM_ASSERT(false && "Loaded month must load workouts...");
    load_prices();
    return true;
}

bool Journal::load_prices()
{
    std::ifstream ifs(generate_prices_name(_current_month, _current_year));
    if (ifs.fail())
    {
        ifs = std::ifstream(generate_prices_name());
        if (ifs.fail()) return false;
        // Fallback to default price
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> _lesson_prices;
    ia >> _lesson_price_skipped;
    ia >> _lesson_price_was_ill;
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

    /* TODO: Remove removed lessons for real? */

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
        _day(workout->should_attend.tm_mday)->set_status(workout->should_lesson, internal_student_id, STATUS_WORKED_OUT);
        int discount_status = _discount_status(current_student.get_contract());
        _day(workout->should_attend.tm_mday)->set_discount_status(workout->should_lesson, internal_student_id, discount_status);
    }

    for (int group_id = 0; group_id < _all_groups.size(); group_id++) // TODO CRITICAL: test this with other months.
    {
        Group* group = _all_groups[group_id];
        for (int student_id = 0; student_id < group->get_size(); student_id++)
        {
            const Student& student = group->get_student(student_id);
            if (group->is_moved_away(student)) group->delete_student(student);
            if (group->is_moved_away(student)) IM_ASSERT(false && "Student wasn't deleted after being 'moved away'");
        }
    }

    load_prices();
    save(); // Needed for generating new months in Add_Working_Out Popup.
}

void Journal::generate_empty()
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

void Journal::generate()
{
    if (this->journal_main == this)
    {
        if (std::filesystem::exists(generate_file_name(_current_month, _current_year)))
        {
            IM_ASSERT(false && "Tried to generate an existing main save file");
        }
    }
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
    load_prices();
    save(); // Needed for generating new months in Add_Working_Out Popup.
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
    if (_state == State::Empty) return;
    bool is_current_year_allocated = _journal_main_bottom_year != Workout_Handler::get_bottom_year(current_month(), current_year());
    if (is_current_year_allocated) delete _workout_handler;
}
