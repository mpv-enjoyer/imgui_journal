#pragma once
#include "../storage/journal.h"

class Popup_Add_Merged_Lesson_To_Journal;
class Popup_Add_Student_To_Base;
class Popup_Add_Student_To_Group;
class Popup_Add_Working_Out;
class Popup_Select_Day_Of_The_Week;
class Popup_Edit_Lesson;

class Subwindow_Students_List;
class Subwindow_Lessons_List;

class Mainwindow;

class Graphical
{
    bool _edit_mode;
    int _wday;
    std::vector<Day_With_Info> _visible_days;

public:
    Popup_Add_Student_To_Group* popup_add_student_to_group = nullptr;
    Popup_Select_Day_Of_The_Week* popup_select_day_of_the_week = nullptr;
    Popup_Add_Merged_Lesson_To_Journal* popup_add_merged_lesson_to_journal = nullptr;
    Popup_Add_Working_Out* popup_add_working_out = nullptr;
    Popup_Add_Student_To_Base* popup_add_student_to_base = nullptr;

    Subwindow_Students_List* subwindow_students_list = nullptr;
    Subwindow_Lessons_List* subwindow_lessons_list = nullptr;

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