#pragma once
#include "model/imodel.h"
#include "controller/icontroller.h"
#include "ui/ui.h"

namespace View
{
    class Subwindow
    {
        const std::string m_id;
    protected:
        static constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        IController& m_controller;
        const IModel& m_model;
        virtual bool render_logic() = 0;
        virtual ImGuiWindowFlags get_window_flags() { return DEFAULT_WINDOW_FLAGS; }
    public:
        Subwindow(IController& controller)
        : m_controller(controller), m_model(m_controller.model())
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