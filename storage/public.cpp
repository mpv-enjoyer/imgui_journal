#include "journal.h"

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
Journal::Journal()
{
    _current_year = _current_time.tm_year;
    _current_month = _current_time.tm_mon;
    _current_month_days_num = get_number_of_days(_current_month, _current_year + 1900);
}
const Student* Journal::student(int id) 
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
    std::vector<Day_With_Info> output;
    int day = get_first_wday(_current_month, _current_year, wday);
    int day_count = get_number_of_days(_current_month, _current_year);
    for ( ; day <= day_count; day+=7)
    {
        Day_With_Info current;
        current.day = _day(day - 1);
        current.number = day;
        current.is_future = day > _current_time.tm_mday;
        current.is_today = day == _current_time.tm_mday;
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
    if (status == STATUS_SKIPPED) return 50;
    if (status == STATUS_WAS_ILL) return 0;
    if (status == STATUS_NOT_AWAITED) return -1;
    int defined_status = _day(mday)->get_discount_status(lesson, internal_student_id);
    int wday = get_wday(mday, _current_month, _current_year);
    Lesson_Pair pair = _all_lessons[wday][lesson.merged_lesson_id]->get_lesson_pair(lesson.internal_lesson_id);
    int lesson_type = pair.lesson_name_id;
    if (defined_status != -1) return _lesson_prices[lesson_type][defined_status];
    //discount status is undefined at this point, fall back to common
    int contract = lesson_info(wday, lesson.merged_lesson_id)->get_group().get_student(internal_student_id).get_contract();
    return lesson_common_price(contract, lesson_type);
}
void Journal::set_student_name(int id, std::string name)
{
    _all_students[id]->set_name(name);
}
void Journal::set_student_age_group(int id, int age_group)
{
    _all_students[id]->set_age_group(age_group);
}
void Journal::set_student_contract(int id, int contract)
{
    _all_students[id]->set_contract(contract);
}
void Journal::set_group_number(int wday, int merged_lesson_id, int number)
{
    _all_lessons[wday][merged_lesson_id]->_group().set_number(number);
}
void Journal::set_group_comment(int wday, int merged_lesson_id, std::string comment)
{
    _all_lessons[wday][merged_lesson_id]->_group().set_comment(comment);
}
void Journal::set_student_attend_data(int wday, int merged_lesson_id, int internal_student_id, Attend_Data new_attend_data)
{
    Lesson current_lesson;
    current_lesson.merged_lesson_id = merged_lesson_id;
    const auto visible_days = _enumerate_days(wday);
    for (int i = 0; i < visible_days.size(); i++)
    {
        if (!(visible_days[i].is_future || visible_days[i].is_today)) continue;
        for (int j = 0; j < 2; j++)
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
void Journal::set_date(int month, int year)
{
    if (month != current_time.tm_mon || year != current_time.tm_year)
        throw std::invalid_argument("not implemented");
    //TODO CRITICAL: check if journal is available for this month and load.
    _current_month = month;
    _current_year = year;
    _current_month_days_num = get_number_of_days(month, year + 1900);
}
void Journal::add_student_to_base(std::string name, int contract)
{
    IM_ASSERT(contract >= 0 && name.size() > 0);
    Student* current = new Student();
    current->set_contract(contract);
    current->set_name(name);
    _all_students.push_back(current);
}
void Journal::add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs)
{
    IM_ASSERT(wday >= 0 && wday <= 6 && number >= 0 && age_group >= -1 && age_group <= AGE_GROUP_COUNT);
    Group* group = new Group();
    group->set_age_group(age_group);
    group->set_comment(comment);
    group->set_number(number);
    _all_groups.push_back(group);
    Lesson_Info* current = new Lesson_Info();
    for (int i = 0; i < MAX_INTERNAL_LESSONS; i++)
        current->add_lesson_pair(lesson_pairs[i]);
    current->set_group(PTRREF(group));
    std::vector<Lesson_Info*>& lessons_in_this_day = std::ref(_all_lessons[wday]);
    int new_merged_lesson_known_id = _emplace_lesson_info(wday, PTRREF(current));
    lessons_in_this_day.insert(lessons_in_this_day.begin() + new_merged_lesson_known_id, current);
    std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
    for (int i = 0; i < affected_days.size(); i++)
    {
        bool await_no_one = false;
        if (!(affected_days[i].is_future || affected_days[i].is_today)) await_no_one = true;
        affected_days[i].day->add_merged_lesson(PTRREF(current), await_no_one, new_merged_lesson_known_id);
    }
}
void Journal::add_student_to_group(int student_id, int wday, int merged_lesson_id)
{
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
            if (!(affected_days[current_day_cell].is_future || affected_days[current_day_cell].is_today)) status = STATUS_NOT_AWAITED;
            affected_days[current_day_cell].day->set_status(current_known_lesson, new_student_id, status);
        }
    }
}
void Journal::add_working_out(const std::tm caller_date, const std::tm select_date, int student_id, Lesson caller_lesson, Lesson select_lesson)
{
    if (caller_date.tm_mon != select_date.tm_mon || caller_date.tm_year != select_date.tm_year)
        throw std::invalid_argument("not implemented");
    Student& student = PTRREF(_all_students[student_id]);
    int internal_student_id = _day(select_date.tm_mday)->find_student(student, select_lesson.merged_lesson_id);
    Workout_Info caller_workout_info;
    caller_workout_info.student = &student;
    caller_workout_info.lesson_info = _all_lessons[select_date.tm_wday][select_lesson.merged_lesson_id];
    caller_workout_info.internal_lesson = select_lesson.internal_lesson_id;
    caller_workout_info.date = select_date;
    caller_workout_info.recovery_hint = select_lesson;
    _day(caller_date.tm_mday)->add_workout(student, caller_lesson, caller_workout_info);
    _day(select_date.tm_mday)->set_status(select_lesson, internal_student_id, STATUS_WORKED_OUT);
    Workout_Info select_workout_info;
    select_workout_info.date = caller_date;
    select_workout_info.lesson_info = _all_lessons[caller_date.tm_wday][caller_lesson.merged_lesson_id];
    select_workout_info.student = &student;
    select_workout_info.internal_lesson = caller_lesson.internal_lesson_id;
    _day(select_date.tm_mday)->insert_workout_into_status(select_lesson, internal_student_id, select_workout_info);
    int discount_status = _discount_status(student.get_contract());
    _day(select_date.tm_mday)->set_discount_status(select_lesson, internal_student_id, discount_status);
}
void Journal::edit_lesson(int wday, int merged_lesson_id, int number, std::string comment, std::vector<Lesson_Pair> pairs)
{
    Lesson_Info& lesson_info = PTRREF(_all_lessons[wday][merged_lesson_id]);
    Group& group = lesson_info._group();
    while (lesson_info.get_lessons_size() != 0)
        lesson_info.delete_lesson_pair(0);
    for (int i = 0; i < pairs.size(); i++)
        lesson_info.add_lesson_pair(pairs[i]);
    
    group.set_number(number);
    group.set_comment(comment);
    _all_lessons[wday].erase(_all_lessons[wday].begin() + merged_lesson_id);
    int new_merged_lesson_id = _emplace_lesson_info(wday, lesson_info);
    //workaround???
    //if (new_index >= merged_lesson_id && new_index != all_lessons[day].size() - 1) new_index++;
    std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
    for (auto current : affected_days)
    {
        current.day->swap_merged_lessons(merged_lesson_id, new_merged_lesson_id);
    }
}
void Journal::append_workout_students(Day_With_Info visible_day, Lesson lesson, std::vector<const Student*>& workout_students)
{
    const Calendar_Day* current_day = visible_day.day;
    for (int workout_num = 0; workout_num < current_day->get_workout_size(lesson); workout_num++)
    {
        const Student* current_workout_student = current_day->get_workout_student(lesson, workout_num);
        bool is_in_vector = false;
        for (int repeat_check_id = 0; repeat_check_id < workout_students.size(); repeat_check_id++)
        {
            if (workout_students[repeat_check_id] == current_workout_student) 
            { 
                is_in_vector = true;
                break;
            }
        }
        if (!is_in_vector) workout_students.push_back(current_workout_student);
    }
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
    _all_students[id]->remove();
}
void Journal::restore_student(int id)
{
    _all_students[id]->restore();
}
void Journal::remove_lesson(int wday, int merged_lesson_id)
{
    _all_lessons[wday][merged_lesson_id]->discontinue();
}
void Journal::restore_lesson(int wday, int merged_lesson_id)
{
    _all_lessons[wday][merged_lesson_id]->restore();
    //TODO CRITICAL: replace NAW's with zeros
}
void Journal::set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed)
{
    Calendar_Day* current_day = _day(mday);
    int contract = lesson_info(wday(mday), lesson.merged_lesson_id)->get_group().get_student(internal_student_id).get_contract();
    int new_discount_status = _discount_status(contract);
    if (status.status != STATUS_WORKED_OUT)
    {
        if (workout_existed)
        {
            Workout_Info distant_workout_info = status.workout_info;
            int day_index = status.workout_info.date.tm_mday - 1;
            int internal_lesson_id = status.workout_info.internal_lesson;
            _day(day_index)->delete_workout(PTRREF(distant_workout_info.lesson_info), internal_lesson_id, PTRREF(distant_workout_info.student));
        }
        current_day->set_status(lesson, internal_student_id, status.status);
        current_day->set_discount_status(lesson, internal_student_id, new_discount_status);
    }
}