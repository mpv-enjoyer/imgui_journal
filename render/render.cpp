#include "render.h"
#include "../platforms/platforms.h"

void Render::change_current_month(int month, int year)
{
    bool is_current_month_main = journal_main == journal;
    bool is_changed_month_main = journal_main->current_month() == month && journal_main->current_year() == year;
    if (is_changed_month_main && is_current_month_main) return;

    int backup_wday = graphical->wday;
    if (is_current_month_main)
    {
        journal->save();
    }
    else
    {
        delete journal;
        delete graphical;
    }

    // Data saved. Now load everything for another month

    if (is_changed_month_main)
    {
        // Months with State::Preview can generate workouts that affect main journal.
        journal_main->load_workouts();
        journal = journal_main;
        graphical = graphical_main;
    }
    else
    {
        journal = new Journal(month, year, journal_main);
        graphical = new JournalHolder(PTRREF(journal));
    }

    graphical->select_wday(backup_wday);
    mainwindow.update_graphical(graphical);
}

Render::Render(Journal* _journal, JournalHolder *_graphical)
 : journal(_journal), graphical(_graphical), mainwindow(graphical, &popup_handler, &subwindow_handler)
{
    IM_ASSERT(Impl::renderer()->is_initialized());
    io = &(ImGui::GetIO());
    IM_ASSERT(Impl::platform()->load_font(io));

    graphical_main = graphical;
    journal_main = journal;
}

void Render::main_loop()
{
    while (!Impl::renderer()->should_close())
    {
        if (Impl::renderer()->is_mouse_button_pressed()) set_poll_time(1);
        if (poll_until >= ImGui::GetTime())
        {
            Impl::renderer()->wait_events_timeout(0.05f);
        }
        else
        {
            Impl::renderer()->wait_events();
            set_poll_time(0.6f);
        }
        if (io->AnyKeyPressed)
        {
            set_poll_time(0.6f);
        }
        show_frame();
    }
}

// the program usually doesn't update the screen by itself
// but if poll_until > GetTime() it updates at some frequency
void Render::set_poll_time(float active_s)
{
    if (ImGui::GetTime() + active_s < poll_until) return;
    poll_until = ImGui::GetTime() + active_s;
}

void Render::show_subwindows()
{
    subwindow_handler.render_subwindow(this);
}

void Render::show_popups()
{
    popup_handler.render_popup(this);
}

void Render::show_frame()
{
    Impl::renderer()->begin_frame();
    mainwindow.show_frame();
    Mainwindow::Callback callback = mainwindow.get_callback();
    if (callback == Mainwindow::Callback::month_left)
    {
        int month = journal->current_month();
        int year = journal->current_year();
        previous_month_for(month, year);
        change_current_month(month, year);
    }
    else if (callback == Mainwindow::Callback::month_right)
    {
        int month = journal->current_month();
        int year = journal->current_year();
        next_month_for(month, year);
        change_current_month(month, year);
    } else if (callback == Mainwindow::Callback::month_default)
    {
        change_current_month(journal_main->current_month(), journal_main->current_year());
    }
    show_subwindows();
    show_popups();

    Impl::renderer()->render_frame();
}

void Render::prepare_shutdown()
{
    Impl::renderer()->cleanup();
}
