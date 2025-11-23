#pragma once
#include "iunit.h"
#include "common/modifiers.h"

namespace UI
{
    struct Scope_Group
    {
        NON_COPYABLE_NOR_MOVABLE(Scope_Group);
        [[nodiscard]] explicit Scope_Group() { ImGui::BeginGroup(); }
        ~Scope_Group() { ImGui::EndGroup(); }
        operator bool() { return true; }
    };

    struct Scope_Child
    {
        NON_COPYABLE_NOR_MOVABLE(Scope_Child);
        bool success;
        [[nodiscard]] explicit Scope_Child(std::string str_id, const ImVec2 &size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags window_flags = 0)
        {
            success = ImGui::BeginChild(str_id.c_str(), size, border, window_flags);
        }
        [[nodiscard]] explicit Scope_Child(ImVec4 color, std::string str_id, const ImVec2 &size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags window_flags = 0)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
            success = ImGui::BeginChild(str_id.c_str(), size, border, window_flags);
            // BG color should still apply before ImGui::End for subwindow
            // Doing this because ChildBg propagates into ImGui::InputText()
            // and I don't want this.
            ImGui::PopStyleColor();
        }
        // (c) Always call a matching EndChild() for each BeginChild() call, regardless of its return value.
        ~Scope_Child() { ImGui::EndChild(); }
        operator bool() { return success; }
    };

    struct Scope_Color
    {
        NON_COPYABLE_NOR_MOVABLE(Scope_Color);
        ImGuiCol idx;
        [[nodiscard]] explicit Scope_Color(ImGuiCol idx, ImU32 col)
        : idx(idx)
        {
            ImGui::PushStyleColor(idx, col);
        }
        [[nodiscard]] explicit Scope_Color(ImGuiCol idx, ImVec4 col)
        : idx(idx)
        {
            ImGui::PushStyleColor(idx, col);
        }
        ~Scope_Color() { ImGui::PopStyleColor(); }
        operator bool() { return true; }
    };

    struct Scope_Color_Input
    {
        Scope_Color scope_color;
        NON_COPYABLE_NOR_MOVABLE(Scope_Color_Input);
        [[nodiscard]] explicit Scope_Color_Input()
        : scope_color(ImGuiCol_FrameBg, ImVec4(ImColor::HSV(0.0f, 0.0f, 0.75f)))
        { }
    };

    bool button_selectable(std::string id, bool selected, bool small = false)
    {
        if (selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
        }
        bool output;
        if (!small) output = ImGui::Button(id.c_str());
        else output = ImGui::SmallButton(id.c_str());
        if (selected)
        {
            ImGui::PopStyleColor(3);
        }
        return output;
    }

    void label(std::string text)
    {
        ImGui::Text("%s", text.c_str());
    }
}