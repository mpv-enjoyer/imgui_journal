#include "journal.h"

const std::vector<std::vector<int>> Journal::get_lesson_prices()
{
    return _lesson_prices;
}
const int Journal::get_ill_price()
{
    return _lesson_price_was_ill;
}
const int Journal::get_skipped_price()
{
    return _lesson_price_skipped;
}
std::string Journal::Lesson_name(int type)
{
    return _lesson_names[type];
}
const int Journal::Lesson_price(int type, int discount_status)
{
    return _lesson_prices[type][discount_status];
}
std::string Journal::Month_name(int number)
{
    return _month_names[number];
}
std::string Journal::Wday_name(int wday)
{
    return _day_names[wday];
}
std::string Journal::Wday_name_short(int wday)
{
    return _day_names_abbreviated[wday];
}
std::string Journal::Age_group(int number)
{
    return _age_group_names[number];
}

bool Journal::_search_last_generated_month(int* month, int* year)
{
    bool is_bottom_year = _current_month >= STUDY_YEAR_BEGIN_MONTH;
    int check_year = _current_year;
    if (!is_bottom_year)
    {
        for (int i = _current_month - 1; i >= 0; i--)
        {
            if (Journal::save_file_exists(i, check_year))
            {
                *month = i;
                *year = check_year;
                return true;
            }
        }
    }
    int check_month_begin = _current_month;
    if (!is_bottom_year)
    {
        check_month_begin = 11;
        check_year--;
    }
    for (int i = check_month_begin; i >= STUDY_YEAR_BEGIN_MONTH; i--)
    {
        if (Journal::save_file_exists(i, check_year))
        {
            *month = i;
            *year = check_year;
            return true;
        }
    }
    return false;
}

// Any month and year
Journal::Journal(int month, int year, Journal* _journal_main)
{
    // Reach the real "main" journal:
    while (_journal_main->journal_main != nullptr) _journal_main = _journal_main->journal_main;
    journal_main = _journal_main;

    _journal_main_bottom_year = Workout_Handler::get_bottom_year(journal_main->current_month(), journal_main->current_year());
    _current_year = year;
    _current_month = month;
    _current_month_days_num = get_number_of_days(_current_month, _current_year + 1900);

    int bottom_year = Workout_Handler::get_bottom_year(month, year);
    if (journal_main->workout_handler()->bottom_year() < bottom_year)
    {
        _state = State::Empty;
        generate_empty();
        return;
    }

    bool is_before = _current_time.tm_year > year;
    is_before |= _current_time.tm_year == year && _current_time.tm_mon > month;
    bool is_current = _current_time.tm_year == year && _current_time.tm_mon == month;
    bool is_future = !(is_before || is_current);
    IM_ASSERT(!is_current && "Check for current before it's generated");
    bool load_result = load();
    if (load_result)
    {
        IM_ASSERT(!is_future);
        // Workouts loaded, journal loaded.
        if (is_before) _state = State::Limited;
        return;
    }
    // Check if current month is in main year or before it:
    bool in_this_year = journal_main->workout_handler()->is_month_here(_current_month, _current_year);
    if (in_this_year)
    {
        _workout_handler = journal_main->workout_handler();
        if (is_future)
        {
            _state = State::Preview;
            generate(journal_main->current_month(), journal_main->current_year());
            return;
        }
    }
    else if (!load_workouts())
    {
        // Cannot load workouts -> year not generated (from the past)
        IM_ASSERT(is_before);
        _state = State::Empty;
        generate_empty();
        return;
    }
    _state = State::Limited;
    int found_month, found_year;
    if (_search_last_generated_month(&found_month, &found_year)) generate(found_month, found_year);
    else generate();
}

// Current month and year
Journal::Journal()
{
    _current_year = _current_time.tm_year;
    _current_month = _current_time.tm_mon;
    _journal_main_bottom_year = Workout_Handler::get_bottom_year(_current_month, _current_year);
    _current_month_days_num = get_number_of_days(_current_month, _current_year + 1900);
    _state = State::Fullaccess;
    bool load_result = load();
    if (load_result) return;
    if (!load_workouts())
    {
        _workout_handler = new Workout_Handler(_current_month, _current_year);
        generate();
        return;
    }
    int month, year;
    if (_search_last_generated_month(&month, &year)) generate(month, year);
    else generate();
}
Workout_Handler* Journal::workout_handler()
{
    return _workout_handler;
}
const Student *Journal::student(int id)
{ 
    return _all_students[id]; 
}
const int Journal::student_count()
{
    return _all_students.size();
}
const Lesson_Info* Journal::lesson_info(int wday, int merged_lesson_id)
{
    return _all_lessons[wday][merged_lesson_id];
}
const int Journal::lesson_info_count(int wday)
{
    return _all_lessons[wday].size();
}
const Calendar_Day* Journal::day(int mday)
{
    return _all_days[mday];
}
const int Journal::day_count()
{
    return _all_days.size();
}
const int Journal::wday(int mday)
{
    return get_wday(mday, _current_month, _current_year); 
}
const int Journal::current_year() { return _current_year; };
const int Journal::current_month() { return _current_month; };
const int Journal::current_month_days_num() { return _current_month_days_num; };
const std::vector<Day_With_Info> Journal::enumerate_days(int wday)
{
    if (_state == State::Empty) return std::vector<Day_With_Info>();
    std::vector<Day_With_Info> output;
    int day = get_first_wday(_current_month, _current_year, wday);
    int day_count = get_number_of_days(_current_month, _current_year);
    for ( ; day <= day_count; day+=7)
    {
        Day_With_Info current;
        current.day = _day(day - 1);
        current.number = day;
        if (_current_time.tm_mon == current_month() && _current_time.tm_year == current_year())
        {
            current.is_future = day > _current_time.tm_mday;
            current.is_today = day == _current_time.tm_mday;
        }
        else
        {
            if (_current_time.tm_year == current_year())
            {
                current.is_future = current_month() > _current_time.tm_mon;
                current.is_today = false;
            }
            else
            {
                current.is_future = current_year() > _current_time.tm_year;
                current.is_today = false;
            }
        }
        output.push_back(current);
    }
    return output;
}
const int Journal::lesson_common_price(int contract, int lesson_type)
{
    int discount_status = _discount_status(contract);
    int price = _lesson_prices[lesson_type][discount_status];
    return price;
}
const int Journal::lesson_current_price(Lesson lesson, int mday, int internal_student_id)
{
    int status = day(mday)->get_status(lesson, internal_student_id).status;
    if (status == STATUS_NO_DATA) return -1;
    if (status == STATUS_SKIPPED) return _lesson_price_skipped;
    if (status == STATUS_WAS_ILL) return _lesson_price_was_ill;
    if (status == STATUS_NOT_AWAITED) return -1;
    int defined_status = _day(mday)->get_discount_status(lesson, internal_student_id);
    int wday = get_wday(mday, _current_month, _current_year);
    Lesson_Pair pair = _all_lessons[wday][lesson.merged_lesson_id]->get_lesson_pair(lesson.internal_lesson_id);
    int lesson_type = pair.lesson_name_id;
    if (defined_status >= get_lesson_prices()[lesson_type].size()) defined_status = get_lesson_prices()[lesson_type].size() - 1; /* Status set, then discounts shrink */
    if (defined_status != -1) return _lesson_prices[lesson_type][defined_status];
    //discount status is undefined at this point, fall back to common
    int contract = lesson_info(wday, lesson.merged_lesson_id)->get_group().get_student(internal_student_id).get_contract();
    return lesson_common_price(contract, lesson_type);
}
void Journal::set_student_name(int id, std::string name)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    _all_students[id]->set_name(name);
}
void Journal::set_student_contract(int id, int contract)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    _all_students[id]->set_contract(contract);
}
void Journal::set_group_age_group(int wday, int merged_lesson_id, int age_group)
{
    if (!_check_rights({ State::Fullaccess })) return;
    _all_lessons[wday][merged_lesson_id]->_group().set_age_group(age_group); 
}
void Journal::remove_student_from_group(int wday, int merged_lesson_id, int student_id)
{
    _all_lessons[wday][merged_lesson_id]->_group().delete_student(PTRREF(_all_students[student_id]));
}
void Journal::restore_student_to_group(int wday, int merged_lesson_id, int student_id)
{
    _all_lessons[wday][merged_lesson_id]->_group().restore_student(PTRREF(_all_students[student_id]));
}
bool Journal::does_group_exist(int wday, int number)
{
    const auto& lesson_infos = _all_lessons[wday];
    for (const auto& lesson_info : lesson_infos)
    {
        if (lesson_info->get_group().get_number() == number && !lesson_info->is_discontinued()) return true;
    }
    return false;
}
void Journal::set_group_number(int wday, int merged_lesson_id, int number)
{
    if (!_check_rights({ State::Fullaccess })) return;
    // Check if new number is unique
    if (does_group_exist(wday, number)) return;
    if (number < 0) return;
    _all_lessons[wday][merged_lesson_id]->_group().set_number(number);
}
void Journal::set_group_comment(int wday, int merged_lesson_id, std::string comment)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    _all_lessons[wday][merged_lesson_id]->_group().set_comment(comment);
}
void Journal::set_student_attend_data(int wday, int merged_lesson_id, int internal_student_id, Attend_Data new_attend_data)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    Lesson current_lesson;
    current_lesson.merged_lesson_id = merged_lesson_id;
    const auto visible_days = _enumerate_days(wday);
    for (int i = 0; i < visible_days.size(); i++)
    {
        for (int j = 0; j < MAX_INTERNAL_LESSONS; j++)
        {
            current_lesson.internal_lesson_id = j;
            Student_Status current_status = visible_days[i].day->get_status(current_lesson, internal_student_id);
            bool await = ( new_attend_data == ATTEND_BOTH ) || ( j == 0 ? new_attend_data == ATTEND_FIRST : new_attend_data == ATTEND_SECOND);
            if (current_status.status == STATUS_NO_DATA && !await)
                visible_days[i].day->set_status(current_lesson, internal_student_id, STATUS_NOT_AWAITED);
            else if (current_status.status == STATUS_NOT_AWAITED && await)
                visible_days[i].day->set_status(current_lesson, internal_student_id, STATUS_NO_DATA);
        }
    }
    _all_lessons[wday][merged_lesson_id]->_group().set_attend_data(internal_student_id, new_attend_data);
}
void Journal::set_teacher_name(int mday, Lesson lesson, std::string name)
{
    _day(mday)->set_teacher_name(lesson, name);
}
void Journal::add_student_to_base(std::string name, int contract)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    IM_ASSERT(contract >= 0 && name.size() > 0);
    Student* current = new Student();
    current->set_contract(contract);
    current->set_name(name);
    _all_students.push_back(current);
}
void Journal::add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    IM_ASSERT(wday >= 0 && wday <= 6 && number >= 0 && age_group >= -1 && age_group <= AGE_GROUP_COUNT);
    Group* group = new Group();
    group->set_age_group(age_group);
    group->set_comment(comment);
    group->set_number(number);
    _all_groups.push_back(group);
    Lesson_Info* current = new Lesson_Info();
    for (int i = 0; i < lesson_pairs.size(); i++)
        current->add_lesson_pair(lesson_pairs[i]);
    current->set_group(PTRREF(group));
    std::vector<Lesson_Info*>& lessons_in_this_day = std::ref(_all_lessons[wday]);
    int new_merged_lesson_known_id = _emplace_lesson_info(wday, PTRREF(current));
    std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
    for (int i = 0; i < affected_days.size(); i++)
    {
        bool await_no_one = false;
        affected_days[i].day->add_merged_lesson(PTRREF(current), await_no_one, new_merged_lesson_known_id);
    }
    _workout_handler->change_lesson_info_position(current_month(), wday, -1, new_merged_lesson_known_id, lessons_in_this_day.size());
}
void Journal::add_student_to_group(int student_id, int wday, int merged_lesson_id)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    Student* student = _all_students[student_id];
    Lesson_Info* merged_lesson = _all_lessons[wday][merged_lesson_id];
    Group& group = merged_lesson->_group();
    int new_student_id = group.add_student(PTRREF(student));
    int first_wday = get_first_wday(_current_month, _current_year, wday);
    std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
    for (int current_day_cell = 0; current_day_cell < affected_days.size(); current_day_cell++)
    {
        affected_days[current_day_cell].day->add_student_to_group(merged_lesson_id, PTRREF(student), new_student_id);
        for (int internal_lesson_id = 0; internal_lesson_id < merged_lesson->get_lessons_size(); internal_lesson_id++)
        {
            Lesson current_known_lesson = {merged_lesson_id, internal_lesson_id};
            int status = STATUS_NO_DATA;
            affected_days[current_day_cell].day->set_status(current_known_lesson, new_student_id, status);
        }
    }
}
void Journal::add_working_out(const std::tm caller_date, const std::tm select_date, int student_id, Lesson caller_lesson, Lesson select_lesson)
{
    if (!_check_rights({State::Limited, State::Preview, State::Fullaccess})) return;
    Student& student = PTRREF(_all_students[student_id]);
    int internal_student_id = _day(select_date.tm_mday)->find_student(student, select_lesson.merged_lesson_id);

    IM_ASSERT(internal_student_id != -1);

    Workout_Info_ workout;
    workout.real_attend = caller_date;
    workout.should_attend = select_date;
    workout.real_lesson = caller_lesson;
    workout.should_lesson = select_lesson;
    workout.real_student_id = student_id;

    workout.real_attend.tm_wday = get_wday(caller_date.tm_mday, caller_date.tm_mon, caller_date.tm_year);
    workout.should_attend.tm_wday = get_wday(select_date.tm_mday, select_date.tm_mon, select_date.tm_year);

    workout.should_student_id = student_id;
    _workout_handler->insert_info(workout);

    _day(select_date.tm_mday)->set_status(select_lesson, internal_student_id, STATUS_WORKED_OUT);
    int discount_status = _discount_status(student.get_contract());
    _day(select_date.tm_mday)->set_discount_status(select_lesson, internal_student_id, discount_status);
}
void Journal::edit_lesson_pairs(int wday, int merged_lesson_id, std::vector<Lesson_Pair> pairs)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    Lesson_Info& lesson_info = PTRREF(_all_lessons[wday][merged_lesson_id]);
    Group& group = lesson_info._group();

    Lesson_Info lesson_info_checked = lesson_info;

    while (lesson_info_checked.get_lessons_size() != 0)
    {
        lesson_info_checked.delete_lesson_pair(0);
    }
    for (int i = 0; i < pairs.size(); i++)
    {
        if (!lesson_info_checked.add_lesson_pair(pairs[i])) return; // Cannot add pair, no changes.
    }

    lesson_info = lesson_info_checked;
    _all_lessons[wday].erase(_all_lessons[wday].begin() + merged_lesson_id);
    int new_merged_lesson_id = _emplace_lesson_info(wday, lesson_info);
    //workaround???
    //if (new_index >= merged_lesson_id && new_index != all_lessons[day].size() - 1) new_index++;
    _workout_handler->change_lesson_info_position(current_month(), wday, merged_lesson_id, new_merged_lesson_id, _all_lessons[wday].size());
    std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
    for (auto current : affected_days)
    {
        current.day->swap_merged_lessons(merged_lesson_id, new_merged_lesson_id);
    }
}

const std::vector<std::vector<std::pair<const Workout_Info_*, const Workout_Info_ *>>> Journal::get_workout_info(int real_wday, int real_merged_lesson, std::vector<int>* student_ids)
{
    return _workout_handler->get_info(current_month(), real_wday, real_merged_lesson, PTRREF(student_ids));
}

const std::vector<std::vector<const Workout_Info_*>> Journal::get_workout_info(int real_wday, Lesson real_lesson, std::vector<int>* student_ids)
{
    std::vector<std::vector<const Workout_Info_ *>> output = _workout_handler->get_info(current_month(), real_wday, real_lesson);
    if (student_ids == nullptr) return output;
    student_ids->clear();
    for (int i = 0; i < output.size(); i++)
    {
        int student_id = -1;
        for (int j = 0; j < output[i].size(); j++)
        {
            if (output[i][j] == nullptr) continue;
            student_id = output[i][j]->real_student_id;
            break;
        }
        IM_ASSERT(student_id != -1);
        student_ids->push_back(student_id);
    }
    return output;
}

const Workout_Info_* Journal::get_workout_info(int should_mday, Lesson should_lesson, int should_student_id)
{
    return _workout_handler->get_info(current_month(), should_mday, should_lesson, should_student_id);
}

bool Journal::_match_lesson_types(int l, int r)
{
    if (l == r) return true;
    if (l == NAME_DESIGN && r == NAME_SPECIALCOURSE) return true;
    if (l == NAME_SPECIALCOURSE && r == NAME_DESIGN) return true;
    if (l == NAME_DESIGN && r == NAME_DRAWING) return true;
    if (l == NAME_DRAWING && r == NAME_DESIGN) return true;
    return false;
}

const bool Journal::is_workout_possible(const Lesson_Info* select_lesson, int select_internal_lesson, int student_id, int caller_lesson_name_id)
{
    if (student_id < 0) return false;
    const Student* current_student = student(student_id);
    if (current_student->is_removed()) return false;
    if (select_lesson->is_discontinued()) return false;
    int internal_student_id = select_lesson->get_group().find_student(PTRREF(current_student));
    if (internal_student_id == -1) return false;
    if (!select_lesson->should_attend(internal_student_id, select_internal_lesson)) return false;
    const Lesson_Pair pair = select_lesson->get_lesson_pair(select_internal_lesson);
    int current_lesson_name_id = pair.lesson_name_id;
    if (_match_lesson_types(caller_lesson_name_id, current_lesson_name_id))
        return true;
    return false;
}
const std::string Journal::merged_lesson_name(int wday, int merged_lesson_id, int internal_student_id)
{
    const auto& merged_lesson = lesson_info(wday, merged_lesson_id);
    const Attend_Data attend_data = merged_lesson->get_group().get_attend_data(internal_student_id);
    if (merged_lesson->get_lessons_size() == 1)
    {
        return Lesson_name(merged_lesson->get_lesson_pair(0).lesson_name_id);
    }
    std::string output;
    int first_lesson_type = merged_lesson->get_lesson_pair(0).lesson_name_id;
    int second_lesson_type = merged_lesson->get_lesson_pair(1).lesson_name_id;
    if (attend_data == ATTEND_FIRST)
        return Lesson_name(first_lesson_type) + " _";
    if (attend_data == ATTEND_SECOND)
        return "_ " + Lesson_name(second_lesson_type);
    if (first_lesson_type == second_lesson_type)
        return Lesson_name(first_lesson_type);
    if (first_lesson_type != second_lesson_type)
        return Lesson_name(first_lesson_type) + "+" + Lesson_name(second_lesson_type);
}
void Journal::remove_student(int id)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    _all_students[id]->remove();
}
void Journal::restore_student(int id)
{
    if (!_check_rights({ State::Fullaccess })) return; 
    _all_students[id]->restore();
}
void Journal::remove_lesson(int wday, int merged_lesson_id)
{
    if (!_check_rights({ State::Fullaccess })) return;
    _all_lessons[wday][merged_lesson_id]->discontinue();
}
void Journal::restore_lesson(int wday, int merged_lesson_id)
{
    if (!_check_rights({ State::Fullaccess })) return;
    _all_lessons[wday][merged_lesson_id]->restore();
    //TODO: replace NAW's with zeros?
}
void Journal::set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed)
{
    if (!_check_rights({ State::Fullaccess, State::Limited, State::Preview })) return;
    // State::Preview shouldn't allow direct access from user because it will not be saved.
    Calendar_Day* current_day = _day(mday);
    const Student& student = lesson_info(wday(mday), lesson.merged_lesson_id)->get_group().get_student(internal_student_id);
    int contract = student.get_contract();
    auto student_iterator = std::find(_all_students.begin(), _all_students.end(), &student);
    auto student_id = student_iterator - _all_students.begin();
    int new_discount_status = _discount_status(contract);
    if (status.status != STATUS_WORKED_OUT)
    {
        if (workout_existed)
        {
            Workout_Info distant_workout_info = status.workout_info;
            int day_index = status.workout_info.date.tm_mday - 1;
            int internal_lesson_id = status.workout_info.internal_lesson;

            const Workout_Info_* workout = _workout_handler->get_info(current_month(), mday, lesson, student_id);
            IM_ASSERT(workout != nullptr);
            _workout_handler->delete_info(workout);
            IM_ASSERT(current_day->get_status(lesson, internal_student_id).status == STATUS_WORKED_OUT);
        }
        current_day->set_status(lesson, internal_student_id, status.status);
        current_day->set_discount_status(lesson, internal_student_id, new_discount_status);
    }
}