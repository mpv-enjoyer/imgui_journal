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
        graphical = new Graphical(PTRREF(journal));
    }

    graphical->select_wday(backup_wday);
    graphical->mainwindow = &mainwindow;
    graphical->mainwindow->update_graphical(graphical);
}

Render::Render(Journal* _journal, Graphical *_graphical)
 : journal(_journal), graphical(_graphical), mainwindow(graphical)
{
    bool renderer_found = impl::begin_init_renderer();
    IM_ASSERT(renderer_found && "No renderer found");
    io = &ImGui::GetIO();
    bool font_loaded = impl::load_font(io);
    IM_ASSERT(font_loaded && "Font cannot load");

    graphical_main = graphical;
    journal_main = journal;

    graphical->mainwindow = &mainwindow;
}

void Render::main_loop()
{
    while (!impl::should_close())
    {
        if (impl::is_mouse_button_pressed()) set_poll_time(1);
        if (poll_until >= ImGui::GetTime())
        {
            impl::wait_events_timeout(0.05f);
        }
        else
        {
            impl::wait_events();
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

void Render::show_frame()
{
    impl::begin_frame();
    graphical->mainwindow->show_frame();
    Mainwindow::Callback callback = graphical->mainwindow->get_callback();
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

    impl::render_frame();
}

void Render::prepare_shutdown()
{
    impl::cleanup();
}
