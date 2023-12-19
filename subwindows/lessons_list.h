#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    std::vector<std::vector<Lesson_Info*>>& all_lessons;
    Popup_Edit_Lesson* popup_edit_lesson = nullptr;
public:
    Subwindow_Lessons_List(std::vector<std::vector<Lesson_Info*>>& all_lessons);
    bool show_frame();
};