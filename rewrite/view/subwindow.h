#pragma once
#include "model/imodel.h"
#include "controller/icontroller.h"
#include "ui/ui.h"

class Subwindow
{
    const std::string m_id;
protected:
    static constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    const IModel& m_model;
    IController& m_controller;
    virtual bool render_logic() = 0;
    virtual ImGuiWindowFlags get_window_flags() { return DEFAULT_WINDOW_FLAGS; }
public:
    Subwindow(const IModel& model, IController& controller)
    : m_model(model), m_controller(controller)
    { }
    virtual bool allow_ontop() { return true; }
    bool render()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::Begin(m_id.c_str(), nullptr, get_window_flags());
        bool result = render_logic();
        ImGui::End();
        return result;
    }
};