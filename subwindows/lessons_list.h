#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    const ImVec4 background = ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
public:
    Subwindow_Lessons_List(JournalHolder *_graphical, Popup_Handler* popup_handler);
    bool show_frame() override;
};