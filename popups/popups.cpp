#include "popups.h"

bool Popup::begin_frame(const char* label)
{
    ImGui::OpenPopup(label);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    popup_count_became_one = popup_count != 1 && ImGui::GetPopupCount() == 1;
    popup_count = ImGui::GetPopupCount();
    any_item_active = ImGui::IsAnyItemActive();
    return ImGui::BeginPopupModal(label, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
}

bool Popup::should_exit()
{
    return (popup_count == 1 && !popup_count_became_one && !any_item_active && ImGui::IsKeyPressed(ImGuiKey_Escape));
}