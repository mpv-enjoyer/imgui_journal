#pragma once
#include "popups.h"

class Popup_Add_Working_Out : public Popup
{
private:
    bool quit_early = false;
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
    int caller_lesson_name_id = -1;
    const Lesson_Info* caller_lesson_info;
    std::vector<std::vector<Lesson>> possible_lessons;
    const Group& current_group = caller_lesson_info->get_group();
    Elements::Picker picker;
public:
    Popup_Add_Working_Out(const std::tm current_lesson_time, Lesson current_lesson, const Lesson_Info* current_lesson_info);
    void update_possible_lessons();
    bool show_frame();
    bool is_ok_possible(bool is_calendar_filled);
    void accept_changes();
};