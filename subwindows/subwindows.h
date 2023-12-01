#pragma once
#include "../main.h"
#include "../popups/popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"

class Subwindow
{
protected:
    bool edit_mode = false;
public:
    Subwindow() { };
};

#include "students_list.h"
#include "lessons_list.h"