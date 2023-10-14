#pragma once
#include "main.h"
#include "helper_functions.h"

class Popup
{
private:
    bool accept_edit = false;
public:
    Popup() { };
    bool check_ok() { return accept_edit; }
    bool cancel() { accept_edit = false; return true; }
    bool ok() { accept_edit = true; return true; }
};

class Popup_Add_Student_To_Group : public Popup
{
private:
    int current_group_id = -1;
    int merged_lesson = -1;
    int current_selected_student = -1;
    ImGuiTextFilter text_filter;
    std::vector<Student>* all_students = nullptr;
    std::vector<Group>* all_groups = nullptr;
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
public:
    Popup_Add_Student_To_Group()
    {        
        for (int i = 0; i < all_students->size(); i++)
        {
            if (all_students->at(i).is_removed()) continue;
            if (all_groups->at(current_group_id).is_in_group(i)) continue;
            possible_student_descriptions.push_back((all_students->at(i).get_name() + " (" + std::to_string(all_students->at(i).get_contract()) + ")"));
            possible_student_ids.push_back(i);
        }
    };
    Popup_Add_Student_To_Group(int current_group_id, std::vector<Student>* all_students, std::vector<Group>* all_groups, int merged_lesson) 
    : current_group_id(current_group_id), all_students(all_students), all_groups(all_groups), merged_lesson(merged_lesson)
    {
        IM_ASSERT(!(current_group_id >= all_groups->size() || current_group_id < 0 || merged_lesson < 0));
        for (int i = 0; i < all_students->size(); i++)
        {
            if (all_students->at(i).is_removed()) continue;
            if (all_groups->at(current_group_id).is_in_group(i)) continue;
            possible_student_descriptions.push_back((all_students->at(i).get_name() + " (" + std::to_string(all_students->at(i).get_contract()) + ")"));
            possible_student_ids.push_back(i);
        }
    };
    int get_merged_lesson() { IM_ASSERT(check_ok()); return merged_lesson; };
    int get_added_student() { IM_ASSERT(check_ok()); return current_selected_student; };
    int get_current_group_id() { IM_ASSERT(check_ok()); return current_group_id; };
    bool show_frame();
    bool is_ok_possible(bool select_visible) { return select_visible && current_selected_student!=-1; }
    void accept_changes(const std::vector<std::vector<Lesson_Info>>& all_lessons,
        std::vector<Calendar_Day>& all_days, int current_mday, std::vector<int> visible_table_columns, int current_day_of_the_week);
};

class Popup_Select_Day_Of_The_Week : public Popup
{
private:
    int day_of_the_week = 0;
    int month = 0;
    int year = 0;
public:
    Popup_Select_Day_Of_The_Week() {};
    Popup_Select_Day_Of_The_Week(int current_day_of_the_week, int current_month, int current_year)
    {
        day_of_the_week = current_day_of_the_week;
        month = current_month;
        year = current_year + 1900;
    }
    bool show_frame();
    bool is_ok_possible() { return true; }
    int get_day_of_the_week() { IM_ASSERT(check_ok()); return day_of_the_week; }
    int get_month() { IM_ASSERT(check_ok()); return month; }
    int get_year() { IM_ASSERT(check_ok()); return year - 1900; }
    void accept_changes(int& current_day_of_the_week, int& current_month, int& current_year); //TODO: here I should update some other currents
};

class Popup_Add_Student_To_Base : public Popup
{
private:
    std::string name = "";
    bool is_date_visible = false;
    int contract = 0;
    int age_group = 0;
public:
    Popup_Add_Student_To_Base() {};
    bool show_frame();
    bool is_ok_possible() { return contract >= 0; }
    void accept_changes(std::vector<Student>& all_students);
};

class Popup_Add_Merged_Lesson_To_Journal : public Popup
{
private:
    int day_of_the_week;
    const std::vector<Group>& all_groups;
    int group_number = 0;
    std::string group_comment;
    int combo_lesson_name_id = 0;
    std::vector<Lesson_Pair> lesson_pairs = std::vector<Lesson_Pair>(2, {0,0,0});
public:
    Popup_Add_Merged_Lesson_To_Journal(const std::vector<Group>& all_groups, int current_day_of_the_week)
    : all_groups(all_groups), day_of_the_week(current_day_of_the_week) {}; 
    bool show_frame();
    bool is_ok_possible()
    {
        for (int i = 0; i < all_groups.size(); i++)
        {
            if (all_groups[i].get_number() == group_number && all_groups[i].get_day_of_the_week() == day_of_the_week) return false;
        }
        if (combo_lesson_name_id == 2 || combo_lesson_name_id == 3)
        {
            bool insane_time = false;
            insane_time = insane_time || lesson_pairs[0].time_begin >= lesson_pairs[0].time_end;
            insane_time = insane_time || lesson_pairs[0].time_end > lesson_pairs[1].time_begin;
            insane_time = insane_time || lesson_pairs[1].time_begin >= lesson_pairs[1].time_end;
            return !insane_time;
        }
        else return lesson_pairs[0].time_begin < lesson_pairs[0].time_end;
    };
    void accept_changes(std::vector<Group>* all_groups, std::vector<Student>* all_students,
    std::vector<std::vector<Lesson_Info>>* all_lessons, std::vector<Calendar_Day>* all_days, const std::vector<int>& visible_table_columns, int current_mday);
};

class Popup_Add_Working_Out : public Popup
{
private:
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
    const std::vector<Student>& all_students;
    const std::vector<Group>& all_groups;
    const std::vector<std::vector<Lesson_Info>>& all_lessons;
    const std::vector<Calendar_Day>& all_days;
    int first_mwday = -1;
    int count_mday = -1;
    int select_student = -1;
    int select_day = -1;
    int select_month = -1;
    int select_year = -1;
    Lesson select_lesson = {-1, -1};
    int caller_month = -1;
    int caller_mday = -1;
    int caller_year = -1;
    Lesson caller_lesson;
    ImGuiTextFilter filter;
public:
    Popup_Add_Working_Out(const std::vector<Student>& all_students, const std::vector<Group>& all_groups, const std::vector<std::vector<Lesson_Info>>& all_lessons, 
    const std::vector<Calendar_Day>& all_days, int current_group_id, const std::tm& current_time, const std::tm& current_lesson_time, const Lesson& current_lesson) :
    all_students(all_students), all_groups(all_groups), all_lessons(all_lessons), all_days(all_days)
    {
        for (int i = 0; i < all_students.size(); i++)
        {
            if (all_students[i].is_removed()) continue;
            if (all_groups[current_group_id].is_in_group(i)) continue;
            possible_student_descriptions.push_back((all_students[i].get_name() + " (" + std::to_string(all_students[i].get_contract()) + ")"));
            possible_student_ids.push_back(i);
        }
        first_mwday = calculate_first_mwday(current_time.tm_mday, current_time.tm_wday);
        count_mday = get_number_of_days(current_time.tm_mon, current_time.tm_year + 1900);
        select_month = current_time.tm_mon;
        select_year = current_time.tm_year;
        caller_lesson = current_lesson;
        caller_mday = current_lesson_time.tm_mday;
        caller_month = current_lesson_time.tm_mon;
        caller_year = current_lesson_time.tm_year;
    }
    bool show_frame();
    bool is_ok_possible() 
    {
        if (select_day == -1) return false;
        if (select_student == -1) return false;
        if (select_lesson == Lesson {-1, -1}) return false;
        Student_Status requested_status = all_days[select_day].get_status(select_lesson, select_student);
        if (requested_status.status == STATUS_WORKED_OUT) return false;
        if (requested_status.status == STATUS_NOT_AWAITED) return false; 
        if (requested_status.status == STATUS_SKIPPED) return false; //STATUS_INVALID will be excluded
        return true;
    };
    void accept_changes(std::vector<Calendar_Day>& all_days);
};