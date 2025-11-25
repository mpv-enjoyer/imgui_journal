#pragma once
#include "subwindow_handler.h"
#include "testwindow.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Lessons_List : public Subwindow
    {
    public:
        const ImVec4 BACKGROUND_COLOR = ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
    private:
        bool render_logic() override
        {
            UI::Scope_Color color(ImGuiCol_ChildBg, BACKGROUND_COLOR);
            if (ImGui::Button("Вернуться к журналу")) return true;
            ImGui::SameLine();
            // TODO: everything here basically
            return false;
        }
    public:
        Lessons_List(IController& controller, Shared& shared)
        : Subwindow("Список всех групп", controller, shared)
        { }
    };
}