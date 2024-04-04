#include "render.h"

#define DEFAULT_POPUP_HANDLER(N, V) \
    if (N) \
    { \
        bool is_done = N->show_frame(); \
        if (is_done && N->check_ok()) \
        { \
            N->accept_changes(V); \
        } \
        if (is_done) \
        { \
            free(N); \
            N = nullptr; \
        } \
        return true; \
    } 

bool Render::show_popups()
{
    DEFAULT_POPUP_HANDLER(graphical->popup_add_merged_lesson_to_journal, );
    DEFAULT_POPUP_HANDLER(graphical->popup_add_student_to_base, );
    DEFAULT_POPUP_HANDLER(graphical->popup_add_student_to_group, );
    DEFAULT_POPUP_HANDLER(graphical->popup_add_working_out, );
    DEFAULT_POPUP_HANDLER(graphical->popup_select_day_of_the_week, );
    return false;
}