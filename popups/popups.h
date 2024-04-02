#pragma once
#include "../main.h"

class Popup
{
private:
    bool accept_edit = false;
    std::string last_error = "";
public:
    bool check_ok() { return accept_edit; }
    void cancel() { accept_edit = false; }
    void ok() { accept_edit = true; }
    void error(std::string desc) { last_error = "ошибка: " + desc; }
    void print_error() { ImGui::TextDisabled(last_error.c_str()); }
};

// Forward declarations

typedef Popup_Add_Merged_Lesson_To_Journal Popup_Add_Merged_Lesson_To_Journal;
typedef Popup_Add_Student_To_Base Popup_Add_Student_To_Base;
typedef Popup_Add_Student_To_Group Popup_Add_Student_To_Group;
typedef Popup_Add_Working_Out Popup_Add_Working_Out;
typedef Popup_Select_Day_Of_The_Week Popup_Select_Day_Of_The_Week;
typedef Popup_Edit_Lesson Popup_Edit_Lesson;

#include "add_merged_lesson_to_journal.h"
#include "add_student_to_base.h"
#include "add_student_to_group.h"
#include "add_working_out.h"
#include "select_day_of_the_week.h"
#include "edit_lesson.h"
#include "confirm.h"