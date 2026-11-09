#pragma once
#include "popups.h"

class Render;

class Popup_Handler
{
    Popup* current = nullptr;
public:
    Popup_Handler() { };
    bool is_popup_opened();
    void open_popup(Popup* popup);
    bool render_popup(Render* render); // Pass in Render to be sure that this is called by Render
};