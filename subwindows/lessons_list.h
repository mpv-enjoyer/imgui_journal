#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    std::vector<std::vector<Lesson_Info*>>& all_lessons;
    std::vector<Calendar_Day*>& all_days;
    int month_num;
    int year_num;
    Popup_Edit_Lesson* popup_edit_lesson = nullptr;
public:
    Subwindow_Lessons_List(std::vector<std::vector<Lesson_Info*>>& all_lessons, std::vector<Calendar_Day*>& all_days, int month, int year);
    bool show_frame();
};