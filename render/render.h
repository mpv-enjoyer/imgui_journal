#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/journal.h"

class Render
{
    Journal* journal;
    Graphical* graphical;
    GLFWwindow* window;
    ImGuiIO* io;
    Mainwindow mainwindow;
    double poll_time;
    void set_update_time(int ms);
    void prepare_first_frame();
    bool show_subwindows();
    bool show_popups();
    void show_frame();
    void prepare_shutdown();
public:
    Render(Journal* _journal, Graphical* _graphical);
    void main_loop();
};