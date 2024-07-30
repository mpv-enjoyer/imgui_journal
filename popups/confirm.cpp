#include "confirm.h"

Popup_Confirm::Popup_Confirm(std::string description, void (*action)())
{
    this->action = action;
    text = "Вы уверены, что хотите " + description + "?";
}

bool Popup_Confirm::show_frame()
{
    if (begin_frame("Подтверждение"))
    {
        ImGui::Text(text.c_str());
        if (ImGui::Button("Да")) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
    }
    ImGui::EndPopup();
}

void Popup_Confirm::activate()
{
    IM_ASSERT(check_ok());
    action();
}