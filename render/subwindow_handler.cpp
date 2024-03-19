#include "render.h"

bool Render::show_subwindows()
{
    bool active = false;
    if (Graphical::subwindow_students_list)
    {
        active = true;
        if (Graphical::subwindow_students_list->show_frame())
        {
            free(Graphical::subwindow_students_list);
            Graphical::subwindow_students_list = nullptr;
        }
    }
    if (Graphical::subwindow_lessons_list)
    {
        active = true;
        if (Graphical::subwindow_lessons_list->show_frame())
        {
            free(Graphical::subwindow_lessons_list);
            Graphical::subwindow_lessons_list = nullptr;
        }
    }
    return active;
}