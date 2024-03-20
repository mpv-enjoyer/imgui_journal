#include "public.h"
#include "internal.h"

namespace Journal
{
    const Student* student(int id) 
    { 
        return _all_students[id]; 
    }
    const int student_count()
    {
        return _all_students.size();
    }
    const Lesson_Info *lesson_info(int wday, int merged_lesson_id)
    {
        return _all_lessons[wday][merged_lesson_id];
    }
    const int lesson_info_count(int wday)
    {
        return _all_lessons[wday].size();
    }
    const Calendar_Day *day(int mday)
    {
        return _all_days[mday];
    }
    const int day_count()
    {
        return _all_days.size();
    }

    const int current_year() { return _current_year; };
    const int current_month() { return _current_month; };
    const int current_month_days_num() { return _current_month_days_num; };
    const std::vector<const Day_With_Info> enumerate_days(int wday)
    {
        std::vector<const Day_With_Info> output;
        int day = get_first_wday(_current_month, _current_year, wday);
        int day_count = get_number_of_days(_current_month, _current_year);
        for ( ; day <= day_count; day+=7)
        {
            Day_With_Info current;
            current.day = _day(day);
            current.number = day;
            current.is_future = day > _current_time.tm_mday;
            current.is_today = day == _current_time.tm_mday;
            output.push_back(current);
        }
        return output;
    }
    const int lesson_common_price(int contract, int lesson_type)
    {
        int discount_status = _discount_status(contract);
        int price = Lesson_Prices[lesson_type][discount_status];
        return price;
    }
    const int lesson_current_price(Lesson lesson, int mday, int internal_student_id)
    {
        int defined_status = _day(mday)->get_discount_status(lesson, internal_student_id);
        int wday = get_wday(mday, _current_month, _current_year);
        Lesson_Pair pair = _all_lessons[wday][lesson.merged_lesson_id]->get_lesson_pair(lesson.internal_lesson_id);
        int lesson_type = pair.lesson_name_id;
        if (defined_status != -1) return Lesson_Prices[lesson_type][defined_status];
    }
    void set_date(int month, int year)
    {
        if (month != current_time.tm_mon || year != current_time.tm_year)
            throw std::invalid_argument("not implemented");
        //TODO CRITICAL: check if journal is available for this month and load.
        _current_month = month;
        _current_year = year;
        _current_month_days_num = get_number_of_days(month, year + 1900);
    }
    void add_student_to_base(std::string name, int contract)
    {
        IM_ASSERT(contract >= 0 && name.size() > 0);
        Student* current = new Student();
        current->set_contract(contract);
        current->set_name(name);
        _all_students.push_back(current);
    }
    void add_merged_lesson(int wday, int number, std::string comment, int age_group, std::vector<Lesson_Pair> lesson_pairs)
    {
        IM_ASSERT(wday >= 0 && wday <= 6 && number >= 0 && age_group >= 0 && age_group <= AGE_GROUP_COUNT);
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
        std::vector<_Day_With_Info> affected_days = _enumerate_days(wday);
        for (int i = 0; i < affected_days.size(); i++)
        {
            bool await_no_one = false;
            if (!(affected_days[i].is_future || affected_days[i].is_today)) await_no_one = true;
            affected_days[i].day->add_merged_lesson(PTRREF(current), await_no_one, new_merged_lesson_known_id);
        }
    }
    void add_student_to_group(int student_id, int wday, int merged_lesson_id)
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
    void add_working_out(const std::tm caller_date, const std::tm select_date, Student& student, Lesson caller_lesson, Lesson select_lesson)
    {
        if (caller_date.tm_mon != select_date.tm_mon || caller_date.tm_year != select_date.tm_year)
            throw std::invalid_argument("not implemented");
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
    void edit_lesson(int wday, int merged_lesson_id, int number, std::string comment, std::vector<Lesson_Pair> pairs)
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
    //
    bool match_lesson_types(int l, int r)
    {
        if (l == r) return true;
        if (l == NAME_DESIGN && r == NAME_SPECIALCOURSE) return true;
        //TODO:...
    }

    bool is_workout_possible_prior(std::tm date, int student_id, int caller_lesson_name_id)
    {
        if (date.tm_mon != _current_month || date.tm_year != _current_year)
            throw std::invalid_argument("not implemented");
        const Student* current_student = student(student_id);
        if (current_student->is_removed()) return false;
        int wday = get_wday(date.tm_mday, date.tm_mon, date.tm_year);
        bool output = false;
        for (int i = 0; i < lesson_info_count(wday); i++)
        {
            const Lesson_Info* merged_lesson = lesson_info(wday, i);
            if (merged_lesson->is_discontinued()) continue;
            int internal_student_id = lesson_info(wday, i)->get_group().find_student(PTRREF(current_student));
            if (internal_student_id == -1) continue;
            for (int j = 0; j < merged_lesson->get_lessons_size(); j++)
            {
                if (!merged_lesson->should_attend(internal_student_id, j)) continue;
                const Lesson_Pair pair = merged_lesson->get_lesson_pair(j);
                int current_lesson_name_id = pair.lesson_name_id;
                if (caller_lesson_name_id == current_lesson_name_id) return true;
                if (caller_lesson_name_id == NAME_DESIGN && )
            }
        }
        
        
        

        return false;
    }
}