#pragma once
#include "model/model.h"
#include "controller/controller.h"
#include "ui/ui.h"

class Subwindow
{
    const std::string m_id;
protected:
    static constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    const Model& m_model;
    Controller& m_controller; 
    virtual bool render_logic() = 0;
    virtual std::optional<ImVec4> get_background_color() { return {}; }
    virtual ImGuiWindowFlags get_window_flags() { return DEFAULT_WINDOW_FLAGS; }
public:
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