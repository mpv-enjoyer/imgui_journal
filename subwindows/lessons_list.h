#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    const ImVec4 background = ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
public:
    Subwindow_Lessons_List(Graphical *_graphical);
    bool show_frame();
};