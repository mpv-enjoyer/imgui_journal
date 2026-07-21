#pragma once
#include "model/imodel.h"
#include "controller/icontroller.h"
#include "view/ui/ui.h"
#include "view/elements/elements.h"
#include "view/popups/popups.h"
#include "view/popups/popup_handler.h"
#include "view/shared.h"

namespace View
{
    class Subwindow_Handler;
    class Subwindow
    {
        const std::string m_id;
        IController& m_controller;
        Shared& m_shared;
    protected:
        static constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        virtual bool render_logic() = 0;
        virtual ImGuiWindowFlags get_window_flags() { return DEFAULT_WINDOW_FLAGS; }
        IController& controller() { return m_controller; }
        const IModel& model() const { return m_controller.model(); }
        Shared& shared() { return m_shared; }
        Popup_Handler& popup_handler() { return m_shared.popup_handler; }
        Subwindow_Handler& subwindow_handler() { return m_shared.subwindow_handler; }
    public:
        Subwindow(std::string id, IController& controller, Shared& shared)
        : m_id(id), m_controller(controller), m_shared(shared)
        { }
        virtual bool allow_ontop() { return true; }
        bool render()
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::Begin(m_id.c_str(), nullptr, get_window_flags());
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            bool result = render_logic();
            ImGui::PopStyleVar();
            ImGui::End();
            if (!result)
            {
                ImGui::ScheduleOneFrame();
            }
            return result;
        }
    };
}