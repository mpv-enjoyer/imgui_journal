#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/journal.h"

class Render
{
    Journal* journal_main;
    Graphical* graphical_main;
    Journal* journal;
    Graphical* graphical;
    ImGuiIO* io;
    Mainwindow mainwindow;
    double poll_until = 3;
    void set_poll_time(float ms);
    bool show_subwindows();
    bool show_popups();
    void show_frame();
    void prepare_shutdown();
    void change_current_month(int month, int year);
public:
    Render(Journal *_journal, Graphical *_graphical);
    void main_loop();
};