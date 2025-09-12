#pragma once
#include "popups/popup_handler.h"
#include "subwindows/subwindow_handler.h"
#include "platforms/platforms.h"
#include "controller/controller_impl.h"
#include "shared.h"

namespace View
{
    class View
    {
        Shared m_shared;
        Popup_Handler m_popup_handler;
        Subwindow_Handler m_subwindow_handler;
        Ptr<IController> m_controller = Ptr<Controller_Impl>::make();
        int m_save_counter = 0;
        const int SAVE_EVERY_N_SECONDS = 300;
        double m_poll_until = 3; // Wait 3 seconds at the start
        void set_poll_time(double active_s)
        {
            // if poll_until > GetTime() the screen updates at some frequency
            m_poll_until = std::max(m_poll_until, ImGui::GetTime() + active_s);
        }
        void prepare_next_frame()
        {
            if (Impl::renderer()->is_mouse_button_pressed()) set_poll_time(1);
            if (m_poll_until >= ImGui::GetTime())
            {
                Impl::renderer()->wait_events_timeout(0.05f);
            }
            else
            {
                Impl::renderer()->wait_events();
                set_poll_time(0.6f);
            }
            if (ImGui::GetIO().AnyKeyPressed) set_poll_time(0.6f);

            if (m_save_counter != static_cast<int>(ImGui::GetTime()) / SAVE_EVERY_N_SECONDS)
            {
                m_save_counter = (int)ImGui::GetTime() / SAVE_EVERY_N_SECONDS;
                bool result = journal->save();
                if (result) _state = Last_Save_State::succeded_auto;
                else _state = Last_Save_State::failed_auto;
                printf("Automatic save at %f seconds.\n", ImGui::GetTime());
            }
        }
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
                prepare_next_frame();
                Impl::renderer()->begin_frame();
                mainwindow.show_frame();
                m_subwindow_handler.render_subwindow();
                m_popup_handler.render_subwindow();
                Impl::renderer()->end_frame();
            }
        }
        Month get_current_month() const
        {

        }
    };
}