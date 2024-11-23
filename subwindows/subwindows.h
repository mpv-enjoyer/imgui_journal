#pragma once
#include "../popups/popups.h"

class Subwindow
{
protected:
    Graphical* graphical = nullptr;
    Journal* journal = nullptr;
    Popup_Handler* popup_handler;
    bool edit_mode = false;
public:
    Subwindow(Graphical* graphical, Popup_Handler* popup_handler);
    void update_graphical(Graphical* graphical);
};

#include "students_list.h"
#include "lessons_list.h"
#include "prices_list.h"
#include "help.h"
#include "mainwindow.h"