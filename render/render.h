#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/public.h"

namespace Graphical
{
    int wday;
    std::vector<Day_With_Info> visible_days;

    Popup_Add_Student_To_Group* popup_add_student_to_group = nullptr;
    Popup_Select_Day_Of_The_Week* popup_select_day_of_the_week = nullptr;
    Popup_Add_Merged_Lesson_To_Journal* popup_add_merged_lesson_to_journal = nullptr;
    Popup_Add_Working_Out* popup_add_working_out = nullptr;
    Popup_Add_Student_To_Base* popup_add_student_to_base = nullptr;
    Popup_Confirm* popup_confirm = nullptr;
    

    Subwindow_Students_List* subwindow_students_list = nullptr;
    Subwindow_Lessons_List* subwindow_lessons_list = nullptr;
}

namespace Render
{
    GLFWwindow* window;
    ImGuiIO* io;
    void prepare_first_frame();
    bool show_subwindows();
    bool show_popups();
    void show_frame();
}