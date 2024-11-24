#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/journal.h"

class Render
{
    Journal* journal_main;
    JournalHolder* graphical_main;
    Journal* journal;
    JournalHolder* graphical;
    ImGuiIO* io;
    Popup_Handler popup_handler;
    Subwindow_Handler subwindow_handler;
    Mainwindow mainwindow;
    double poll_until = 3;
    void set_poll_time(float ms);
    void show_subwindows();
    void show_popups();
    void show_frame();
    void prepare_shutdown();
    void change_current_month(int month, int year);
public:
    Render(Journal *_journal, JournalHolder *_graphical);
    void main_loop();
};