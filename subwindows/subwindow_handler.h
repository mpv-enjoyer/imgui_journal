#pragma once
#include "subwindows.h"

class Render;

class Subwindow_Handler
{
    std::vector<Subwindow*> current;
public:
    Subwindow_Handler() { };
    bool is_subwindow_opened();
    void open_subwindow(Subwindow* subwindow);
    bool render_subwindow(Render* render); // Pass in Render to be sure that this is called by Render
};