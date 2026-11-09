#pragma once
#include "subwindows.h"

void show_button_to_recalculate_all_prices();
void show_button_to_open_workout_debugging();

class Subwindow_Help : public Subwindow
{
    const ImVec4 background = ImVec4(225.f/255.f, 240.f/255.f, 253.f/255.f, 1.0f);
public:
    Subwindow_Help(JournalHolder *graphical, Popup_Handler* popup_handler);
    void draw_note(std::string text);
    bool show_frame() override;
    bool virtual allow_ontop() override { return false; };
};