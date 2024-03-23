#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    Popup_Edit_Lesson* popup_edit_lesson = nullptr;
public:
    Subwindow_Lessons_List();
    bool show_frame();
};