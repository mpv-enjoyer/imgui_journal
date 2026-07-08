#pragma once
#include "popups/popup_handler.h"
#include "subwindows/subwindow_handler.h"
#include "subwindows/mainwindow.h"
#include "platforms/platforms.h"
#include "controller/controller_impl.h"
#include "controller/commands/commands.h"
#include "shared.h"

namespace View
{
    class Timers
    {
        int m_save_counter = 0;
        const int SAVE_EVERY_N_SECONDS = 300;
        double m_poll_until = 3; // Wait 3 seconds at the start
        void set_poll_time(double active_s, double current_time)
        {
            // if poll_until > GetTime() the screen updates at some frequency
            m_poll_until = std::max(m_poll_until, current_time + active_s);
        }
    public:
        void prepare_next_frame(Ptr<IController>& controller, double current_time, Time_State::Bits time_state_TEMP)
        {
            if (Impl::renderer()->is_mouse_button_pressed()) set_poll_time(1, current_time);
            if (m_poll_until >= current_time)
            {
                Impl::renderer()->wait_events_timeout(0.05f);
            }
            else
            {
                Impl::renderer()->wait_events();
                set_poll_time(0.6f, current_time);
            }
            if (ImGui::GetIO().AnyKeyPressed) set_poll_time(0.6f, current_time);
            
            int want_save_counts = static_cast<int>(current_time) / SAVE_EVERY_N_SECONDS;
            if (m_save_counter != want_save_counts)
            {
                m_save_counter = want_save_counts;
                controller->add(Ptr<Save>::make());
                controller->flush(time_state_TEMP);
                // Flush immediately so the save() command doesn't ever
                // get stacked with some other command in weird combination...

                // TODO: this should really be the method of IController.
            }
        }
    };

    class View
    {
        NON_COPYABLE_NOR_MOVABLE(View);
        Timers m_timers;
        Subwindow_Handler m_subwindow_handler;
        Popup_Handler m_popup_handler;
        Shared m_shared = Shared(m_subwindow_handler, m_popup_handler);
        Ptr<IController> m_controller = Ptr<Controller_Impl>::make();
        Mainwindow m_mainwindow = Mainwindow(*m_controller.get(), m_shared);
    public:
        View()
        {
            // First time calling renderer + init check:
            bool init = Impl::renderer()->is_initialized();
            IM_ASSERT(init);
            Impl::renderer()->begin_frame(false);
            bool dummy;
            draw_frame(dummy);
            Impl::renderer()->end_frame();
        }
        void draw_frame(bool& done)
        {
            if (!m_subwindow_handler.is_subwindow_opened())
            {
                if (m_mainwindow.render()) done = true;
            }
            m_subwindow_handler.render_subwindow();
            m_popup_handler.render_popup();
        }
        void main_loop()
        {
            bool done = false;
            while (!done)
            {
                //m_timers.prepare_next_frame(m_controller, ImGui::GetTime(), Time_State::make(m_shared.month));
                // TODO: transfer autosaves
                bool valid_frame = false;
                while (!valid_frame)
                {
                    Impl::renderer()->wait_events();
                    valid_frame = Impl::renderer()->begin_frame(true);
                    if (Impl::renderer()->should_close()) break;
                }
                if (Impl::renderer()->should_close()) break;
                draw_frame(done);
                Impl::renderer()->end_frame();
                m_controller->flush(Time_State::make(m_shared.month));
            }
        }
        ~View()
        {
            Impl::renderer()->cleanup();
        }
    };
}