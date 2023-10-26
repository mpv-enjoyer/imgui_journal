#pragma once
#include "popups.h"

class Popup_Add_Working_Out : public Popup
{
private:
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
    const std::vector<Student*>& all_students;
    Group& current_group;
    std::vector<std::vector<Lesson_Info*>>& all_lessons;
    const std::vector<Calendar_Day*>& all_days;
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
    Popup_Add_Working_Out(const std::vector<Student*>& all_students, std::vector<std::vector<Lesson_Info*>>& all_lessons, const std::vector<Calendar_Day*>& all_days,
    Group& current_group, const std::tm& current_time, const std::tm& current_lesson_time, Lesson current_lesson);
    bool show_frame();
    bool is_ok_possible(bool is_calendar_filled);
    void accept_changes(std::vector<Calendar_Day*>& all_days);
};