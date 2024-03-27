#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/public.h"

namespace Graphical
{
    const int wday();
    const std::vector<Day_With_Info>& visible_days();
    void select_wday(int wday);
    bool is_edit_mode();
    void set_edit_mode(bool value);
    void init();

    static Popup_Add_Student_To_Group* popup_add_student_to_group;
    static Popup_Select_Day_Of_The_Week* popup_select_day_of_the_week;
    static Popup_Add_Merged_Lesson_To_Journal* popup_add_merged_lesson_to_journal;
    static Popup_Add_Working_Out* popup_add_working_out;
    static Popup_Add_Student_To_Base* popup_add_student_to_base;
    static Popup_Confirm* popup_confirm;

    static Subwindow_Students_List* subwindow_students_list;
    static Subwindow_Lessons_List* subwindow_lessons_list;
}

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

namespace Render
{
    static GLFWwindow* window;
    static ImGuiIO* io;
    static double poll_time;
    void main_loop();
    void set_update_time(int ms);
    void prepare_first_frame();
    bool show_subwindows();
    bool show_popups();
    void show_frame();
    void prepare_shutdown();
}