#include "render.h"

#define DEFAULT_SUBWINDOW_HANDLER(N) \
    if (N) \
    { \
        if (N->show_frame()) \
        { \
            free(N); \
            N = nullptr; \
        } \
        return true; \
    } 

bool Render::show_subwindows()
{
    DEFAULT_SUBWINDOW_HANDLER(graphical->subwindow_lessons_list);
    DEFAULT_SUBWINDOW_HANDLER(graphical->subwindow_students_list);
    DEFAULT_SUBWINDOW_HANDLER(graphical->subwindow_prices_list);
    DEFAULT_SUBWINDOW_HANDLER(graphical->subwindow_help);
    return false;
}