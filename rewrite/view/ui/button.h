#pragma once
#include "aunit.h"
#include "helpers.h"
#include <functional>
#include <optional>

namespace UI
{
    class Button : public AUnit
    {
    public:
        enum class Colors
        {
            Common,
            Dangerous,
            Good
        };
    private:
        std::function<void()> m_callback;
        Colors m_colors;
        static bool apply_colors(Button::Colors colors)
        {
            ImVec4 bg_color;
            ImVec4 hovered_color;
            ImVec4 active_color;
            switch (colors)
            {
            case Colors::Common: return false;
            case Colors::Dangerous:
                bg_color = ImColor::HSV(7.0f / 7.0f, 0.7f, 0.7f);
                hovered_color = ImColor::HSV(7.0f / 7.0f, 0.8f, 0.8f);
                active_color = ImColor::HSV(7.0f / 7.0f, 0.9f, 0.9f);
                break;
            case Colors::Good:
                bg_color = ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f);
                hovered_color = ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f);
                active_color = ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f);
                break;
            default:
                assert(false);
            }
            ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_color);
            return true;
        }
    public:
        Button(std::string id, const Updater* updater = nullptr, std::function<void()> callback = []() -> void {}, Colors colors = Colors::Common)
        : AUnit(id, updater), m_callback(callback), m_colors(colors)
        { }
        void update() { };
    protected:
        void render_logic() override
        {
            bool applied = apply_colors(m_colors);
            bool result = ImGui::Button(m_id.c_str(), {m_width, 0});
            if (!applied) 
            if (result) m_callback();
        }
    };
}