#pragma once
#include "model/imodel.h"
#include "controller/icontroller.h"
#include "view/ui/ui.h"
#include "view/elements/elements.h"
#include "view/popups/popups.h"

namespace View
{
    class Subwindow_Handler;
    class Subwindow
    {
        const std::string m_id;
    protected:
        static constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        IController& m_controller;
        const IModel& m_model;
        Popup_Handler& m_popup_handler;
        Subwindow_Handler& m_subwindow_handler;
        virtual bool render_logic() = 0;
        virtual ImGuiWindowFlags get_window_flags() { return DEFAULT_WINDOW_FLAGS; }
        Popup_Handler& popup_handler() { return m_popup_handler; }
        Subwindow_Handler& subwindow_handler() { return m_subwindow_handler; }
    public:
        Subwindow(std::string id, IController& controller, Popup_Handler& popup_handler, Subwindow_Handler& subwindow_handler)
        : m_id(id), m_controller(controller), m_model(m_controller.model()), m_popup_handler(popup_handler), m_subwindow_handler(subwindow_handler)
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
            return result;
        }
    };
}