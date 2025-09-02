#pragma once
#include "iunit.h"
#include "common/modifiers.h"

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