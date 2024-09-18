#pragma once
#include "subwindows.h"

class Subwindow_Help : public Subwindow
{
    const ImVec4 background = ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
public:
    Subwindow_Help(Graphical *_graphical);
    bool show_frame();
};