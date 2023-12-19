#include "select_day_of_the_week.h"

Popup_Select_Day_Of_The_Week::Popup_Select_Day_Of_The_Week(int current_day_of_the_week, int current_month, int current_year)
{
    day_of_the_week = current_day_of_the_week;
    month = current_month;
    year = current_year + 1900;
}

bool Popup_Select_Day_Of_The_Week::show_frame()
{
    POPUP_INIT_FRAME("Выбрать день недели")
    {
        for (int i = 1; i < 7; i++)
        {
            std::string button_name = generate_label(Day_Names[i] + "##change_day_button", {});
            if (j_button_selectable(button_name.c_str(), day_of_the_week == i)) day_of_the_week = i;
            ImGui::SameLine();
        }
        //now for russian-styled calendar
        std::string button_name = generate_label(Day_Names[0] + "##change_day_button", {});
        if (j_button_selectable(button_name.c_str(), day_of_the_week == 0)) day_of_the_week = 0;
        ImGui::Combo("##Month", &month, Month_Names_Combo);
        ImGui::SameLine();
        ImGui::InputInt("##Year", &year, 0, 0);
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Select_Day_Of_The_Week::accept_changes(int& current_day_of_the_week, int& current_month, int& current_year)
{
    current_day_of_the_week = get_day_of_the_week();
    current_month = get_month();
    current_year = get_year();
    //TODO: current month days num is unsynced
    //...
}