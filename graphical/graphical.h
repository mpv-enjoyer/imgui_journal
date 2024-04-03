#pragma once
#include "../storage/journal.h"

typedef Popup_Add_Merged_Lesson_To_Journal Popup_Add_Merged_Lesson_To_Journal;
typedef Popup_Add_Student_To_Base Popup_Add_Student_To_Base;
typedef Popup_Add_Student_To_Group Popup_Add_Student_To_Group;
typedef Popup_Add_Working_Out Popup_Add_Working_Out;
typedef Popup_Select_Day_Of_The_Week Popup_Select_Day_Of_The_Week;
typedef Popup_Edit_Lesson Popup_Edit_Lesson;

typedef Subwindow_Students_List Subwindow_Students_List;
typedef Subwindow_Lessons_List Subwindow_Lessons_List;

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

    Journal& journal;

    Graphical(Journal& main_journal);
    const int& wday = std::ref(_wday);
    const std::vector<Day_With_Info>& visible_days = std::ref(_visible_days);
    const bool& edit_mode = std::ref(_edit_mode);
    void select_wday(int wday);
    void set_edit_mode(bool value);
};