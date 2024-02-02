#pragma once
#include "popups.h"

class Popup_Confirm : public Popup
{
    std::string text;
    void (*action)();
public:
    Popup_Confirm(std::string description, void (*action)());
    bool show_frame();
    void activate();
};