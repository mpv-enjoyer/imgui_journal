#pragma once
#include "../storage/journal.h"

class Mainwindow;

/*
    JournalHolder (previously Graphical) is:
    1) (V) Journal storage.
    2) (X) Subwindow handler
    3) Some info about user inputs: wday and edit_mode
    4) (X) additional ImGui functions
*/

namespace Graphical
{
    bool attend_data(std::string label, Attend_Data *attend_data, std::string first_lesson_name, std::string second_lesson_name);
    bool button_selectable(const char *label, bool selected, bool small = false);
    bool button_dangerous(const char *label);
    bool input_time(std::string label, JTime &time);
    bool button_colored(const char *label, float r, float g, float b);
    bool age_group_combo(const char *label, int *age_group, bool shrink = true);
    bool attendance_combo(const char* label, int* status, std::string tooltip = "");
};

class JournalHolder
{
    bool _edit_mode;
    int _wday;
    std::vector<Day_With_Info> _visible_days;
public:
//    Mainwindow* mainwindow = nullptr;
    Journal& journal;

    JournalHolder(Journal& main_journal);
    const int& wday = std::ref(_wday);
    const std::vector<Day_With_Info>& visible_days = std::ref(_visible_days);
    const bool& edit_mode = std::ref(_edit_mode);
    void select_wday(int wday);
    void set_edit_mode(bool value);
};