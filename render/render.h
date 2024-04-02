#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/journal.h"

class Graphical
{
    bool _edit_mode;
    int _wday;
    std::vector<Day_With_Info> _visible_days;
public:
    Graphical();
    const int wday();
    const std::vector<Day_With_Info>& visible_days();
    void select_wday(int wday);
    bool is_edit_mode();
    void set_edit_mode(bool value);
}

class Render
{
    Popup_Add_Student_To_Group* popup_add_student_to_group;
    Popup_Select_Day_Of_The_Week* popup_select_day_of_the_week;
    Popup_Add_Merged_Lesson_To_Journal* popup_add_merged_lesson_to_journal;
    Popup_Add_Working_Out* popup_add_working_out;
    Popup_Add_Student_To_Base* popup_add_student_to_base;
    Popup_Confirm* popup_confirm;

    Subwindow_Students_List* subwindow_students_list;
    Subwindow_Lessons_List* subwindow_lessons_list;

    Journal journal;

    namespace Elements
    {
        bool button_selectable(const char* label, bool selected, bool small = false);
        bool button_dangerous(const char *label);
        bool input_time(std::string label, JTime &time);
        bool attendance_combo(const char* label, int* status, std::string tooltip = "");
        bool attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name);
        void table(int merged_lesson_id);
        bool table_row(int merged_lesson_id, int internal_student_id, int counter);
        int table_cell(int merged_lesson_id, int internal_student_id, int visible_day_id);
        void table_add_student_row(int merged_lesson_id, int counter);
        void table_add_workout_row(int merged_lesson_id, int counter);
    }
    static double poll_time;
    void set_update_time(int ms);
    void prepare_first_frame();
    bool show_subwindows();
    bool show_popups();
    void show_frame();
    void prepare_shutdown();
public:
    Render();
    void main_loop();
}