#pragma once
#include "../storage/journal.h"

class Subwindow_Students_List;
class Subwindow_Lessons_List;
class Subwindow_Prices_List;
class Subwindow_Help;

class Mainwindow;

class Graphical
{
    bool _edit_mode;
    int _wday;
    std::vector<Day_With_Info> _visible_days;
public:
    Subwindow_Students_List* subwindow_students_list = nullptr;
    Subwindow_Lessons_List* subwindow_lessons_list = nullptr;
    Subwindow_Prices_List* subwindow_prices_list = nullptr;
    Subwindow_Help* subwindow_help = nullptr;

    Mainwindow* mainwindow = nullptr;

    Journal& journal;

    Graphical(Journal& main_journal);
    const int& wday = std::ref(_wday);
    const std::vector<Day_With_Info>& visible_days = std::ref(_visible_days);
    const bool& edit_mode = std::ref(_edit_mode);
    void select_wday(int wday);
    void set_edit_mode(bool value);

    bool attend_data(std::string label, Attend_Data *attend_data, std::string first_lesson_name, std::string second_lesson_name);
    bool button_selectable(const char *label, bool selected, bool small);
    bool button_dangerous(const char *label);
    bool input_time(std::string label, JTime &time);
    bool button_colored(const char *label, float r, float g, float b);
};