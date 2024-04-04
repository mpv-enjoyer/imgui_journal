#pragma once
#include "../popups/popups.h"

class Subwindow
{
protected:
    Graphical* graphical = nullptr;
    Journal* journal = nullptr;
    bool edit_mode = false;
public:
    Subwindow() { };
};

#include "students_list.h"
#include "lessons_list.h"
#include "mainwindow.h"