#include "render.h"
#include "internal.h"

#define DEFAULT_POPUP_HANDLER(N, V) \
    if (N()) \
    { \
        bool is_done = N()->show_frame(); \
        if (is_done && N()->check_ok()) \
        { \
            N()->accept_changes(V); \
        } \
        if (is_done) \
        { \
            free(N()); \
            N() = nullptr; \
        } \
        return true; \
    } 



bool Render::show_popups()
{
    DEFAULT_POPUP_HANDLER(Graphical::popup_add_merged_lesson_to_journal, );
    DEFAULT_POPUP_HANDLER(Graphical::popup_add_student_to_base, );
    DEFAULT_POPUP_HANDLER(Graphical::popup_add_student_to_group, );
    DEFAULT_POPUP_HANDLER(Graphical::popup_add_working_out, );
    if (Graphical::popup_confirm())
    {
        throw std::invalid_argument("not implemented.");
    }
    if (Graphical::popup_select_day_of_the_week())
    {
        bool is_done = Graphical::popup_select_day_of_the_week()->show_frame();
        if (is_done && Graphical::popup_select_day_of_the_week()->check_ok())
        {
            int new_wday;
            Graphical::popup_select_day_of_the_week()->accept_changes(std::ref(new_wday));
            Graphical::select_wday(new_wday);
        }
        if (is_done)
        {
            free(Graphical::popup_select_day_of_the_week());
            Graphical::popup_select_day_of_the_week() = nullptr;
        }
        return true;
    }
    return false;
}