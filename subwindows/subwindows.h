#pragma once
#include "../popups/popups.h"

class Subwindow
{
protected:
    JournalHolder* graphical = nullptr;
    Journal* journal = nullptr;
    Popup_Handler* popup_handler;
    bool edit_mode = false;
public:
    Subwindow(JournalHolder* graphical, Popup_Handler* popup_handler);
    void update_graphical(JournalHolder* graphical);
    virtual bool show_frame() = 0;
    virtual bool allow_ontop() { return true; };
    virtual ~Subwindow() { };
};

#include "subwindow_handler.h"
#include "students_list.h"
#include "lessons_list.h"
#include "prices_list.h"
#include "help.h"
#include "mainwindow.h"