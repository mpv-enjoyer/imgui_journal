#include "render.h"

Popup_Add_Student_To_Group* _popup_add_student_to_group = nullptr;
Popup_Select_Day_Of_The_Week* _popup_select_day_of_the_week = nullptr;
Popup_Add_Merged_Lesson_To_Journal* _popup_add_merged_lesson_to_journal = nullptr;
Popup_Add_Working_Out* _popup_add_working_out = nullptr;
Popup_Add_Student_To_Base* _popup_add_student_to_base = nullptr;
Popup_Confirm* _popup_confirm = nullptr;

Subwindow_Students_List* _subwindow_students_list = nullptr;
Subwindow_Lessons_List* _subwindow_lessons_list = nullptr;

namespace Graphical
{
#define POPUP_DEFAULT_FUNC(T, N) T*& N() { return std::ref(_ ## N); }

POPUP_DEFAULT_FUNC(Popup_Add_Student_To_Base, popup_add_student_to_base);
POPUP_DEFAULT_FUNC(Popup_Add_Student_To_Group, popup_add_student_to_group);
POPUP_DEFAULT_FUNC(Popup_Select_Day_Of_The_Week, popup_select_day_of_the_week);
POPUP_DEFAULT_FUNC(Popup_Add_Merged_Lesson_To_Journal, popup_add_merged_lesson_to_journal);
POPUP_DEFAULT_FUNC(Popup_Add_Working_Out, popup_add_working_out);
POPUP_DEFAULT_FUNC(Popup_Confirm, popup_confirm);

POPUP_DEFAULT_FUNC(Subwindow_Students_List, subwindow_students_list);
POPUP_DEFAULT_FUNC(Subwindow_Lessons_List, subwindow_lessons_list);
}