#pragma once
#include "popups/popup_handler.h"
#include "subwindows/subwindow_handler.h"
#include "subwindows/mainwindow.h"
#include "platforms/platforms.h"
#include "controller/controller_impl.h"
#include "controller/save.h"
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
        void prepare_next_frame(Ptr<IController>& controller, double current_time)
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
                controller->flush();
                // Flush immediately so the save() command doesn't ever
                // get stacked with some other command in weird combination...
            }
        }
    };

    class View
    {
        Timers m_timers;
        Shared m_shared;
        Popup_Handler m_popup_handler;
        Subwindow_Handler m_subwindow_handler;
        Ptr<IController> m_controller = Ptr<Controller_Impl>::make();
        Mainwindow m_mainwindow = Mainwindow(*m_controller.get(), m_shared);
    public:
        View()
        {
            // First time calling renderer + init check:
            bool init = Impl::renderer()->is_initialized();
            IM_ASSERT(init);
        }
        void main_loop()
        {
            while (!Impl::renderer()->should_close())
            {
                m_timers.prepare_next_frame(m_controller, ImGui::GetTime());
                Impl::renderer()->begin_frame();
                m_mainwindow.render();
                m_subwindow_handler.render_subwindow();
                m_popup_handler.render_subwindow();
                Impl::renderer()->end_frame();
                m_controller->flush();
            }
        }
    };
}