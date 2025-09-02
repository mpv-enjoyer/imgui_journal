#pragma once
#include "aunit.h"
#include "helpers.h"
#include <functional>
#include <optional>

namespace UI
{
    class Button : public AUnit
    {
        std::function<void()> m_callback;
        std::optional<ImVec4> m_bg_color;
        std::optional<ImVec4> m_hovered_color;
        std::optional<ImVec4> m_active_color;
    public:
        Button(std::string id, std::function<void()> callback = []() -> void {}, std::optional<ImVec4> bg_color = std::nullopt, std::optional<ImVec4> hovered_color = std::nullopt, std::optional<ImVec4> active_color = std::nullopt)
        : AUnit(id), m_callback(callback), m_bg_color(bg_color), m_hovered_color(hovered_color), m_active_color(active_color)
        { }
        void update() { };
    protected:
        void render_logic() override
        {
            if (m_bg_color) ImGui::PushStyleColor(ImGuiCol_Button, *m_bg_color);
            if (m_hovered_color) ImGui::PushStyleColor(ImGuiCol_ButtonHovered, *m_hovered_color);
            if (m_active_color) ImGui::PushStyleColor(ImGuiCol_ButtonActive, *m_active_color);
            bool result = ImGui::Button(m_id.c_str(), {m_width, 0});
            if (m_bg_color) ImGui::PopStyleColor();
            if (m_hovered_color) ImGui::PopStyleColor();
            if (m_active_color) ImGui::PopStyleColor();
            if (result) m_callback();
        }
    };
}