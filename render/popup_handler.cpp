#include "render.h"
#include "internal.h"

bool Render::show_popups()
{
    if (Graphical::popup_add_merged_lesson_to_journal)
    {
        bool is_done = Graphical::popup_add_merged_lesson_to_journal->show_frame();
        if (is_done && Graphical::popup_add_merged_lesson_to_journal->check_ok())
        {
            Graphical::popup_add_merged_lesson_to_journal->accept_changes();
        }
        if (is_done)
        {
            free(Graphical::popup_add_merged_lesson_to_journal);
            Graphical::popup_add_merged_lesson_to_journal = nullptr;
        }
        return true;
    }
    if (Graphical::popup_add_student_to_group)
    {
        bool is_done = Graphical::popup_add_student_to_group->show_frame();
        if (is_done && Graphical::popup_add_student_to_group->check_ok())
        {
            Graphical::popup_add_student_to_group->accept_changes();
        };
        if (is_done) 
        {
            free(Graphical::popup_add_student_to_group);
            Graphical::popup_add_student_to_group = nullptr;
        }
        return true;
    }
    if (Graphical::popup_add_working_out)
    {
        bool pressed_ok = Graphical::popup_add_working_out->show_frame();
        if (pressed_ok && Graphical::popup_add_working_out->check_ok())
        {
            Graphical::popup_add_working_out->accept_changes();
        }
        if (pressed_ok)
        {
            free(Graphical::popup_add_working_out);
            Graphical::popup_add_working_out = nullptr;
        }
        return true;
    }
    if (Graphical::popup_confirm)
    {
        throw std::invalid_argument("not implemented.");
    }
    if (Graphical::popup_select_day_of_the_week)
    {
        bool is_done = Graphical::popup_select_day_of_the_week->show_frame();
        if (is_done && Graphical::popup_select_day_of_the_week->check_ok())
        {
            Graphical::popup_select_day_of_the_week->accept_changes(_wday);
        }
        if (is_done)
        {
            free(Graphical::popup_select_day_of_the_week);
            Graphical::popup_select_day_of_the_week = nullptr;
        }
        return true;
    }
    if (Graphical::popup_add_student_to_base)
    {
        const bool result = Graphical::popup_add_student_to_base->show_frame();
        if (result && Graphical::popup_add_student_to_base->check_ok())
        {
            Graphical::popup_add_student_to_base->accept_changes();
        }
        if (result)
        {
            free(Graphical::popup_add_student_to_base);
            Graphical::popup_add_student_to_base = nullptr;
        }
        return true;
    }
    return false;
}