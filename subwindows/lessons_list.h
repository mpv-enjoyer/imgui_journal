#pragma once
#include "subwindows.h"

class Subwindow_Lessons_List : public Subwindow
{
    const bool edit_mode = true;
public:
    Subwindow_Lessons_List(Graphical *_graphical);
    bool show_frame();
};