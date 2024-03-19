#include "add_student_to_base.h"

bool Popup_Add_Student_To_Base::show_frame()
{
    POPUP_INIT_FRAME("Добавить ученика в базу")
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        ImGui::InputText("ФИ ученика", &name);
        if (ImGui::InputInt("No договора", &contract, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
        {
            if (contract < 0) contract = 0;
        }
        ImGui::Checkbox("Указать возрастную группу", &is_date_visible);
        if (is_date_visible)
        {
            ImGui::Combo("##Возрастная группа", &age_group, Age_Group_Names_Combo);
        }
        ImGui::PopStyleColor(1);
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Student_To_Base::accept_changes()
{
    IM_ASSERT(check_ok());
    Journal::add_student_to_base(name, contract);
}