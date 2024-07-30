#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Confirm : public Popup
{
    std::string text;
    void (*action)();
public:
    Popup_Confirm(std::string description, void (*action)());
    bool show_frame();
    void activate();
};