#include "main.h"

const char* c_str_int(int num)
{
    return std::to_string(num).c_str(); 
}

std::string to_string(JTime value)
{
    std::string output;
    output.append(std::to_string(value.hours));
    output.append(":");
    if (value.minutes < 10) output.append("0");
    output.append(std::to_string(value.minutes));
    return output;
}

std::string to_string(JTime begin, JTime end)
{
    return to_string(begin) + " - " + to_string(end);
}

std::string to_string(std::tm day, JTime begin, JTime end, bool abbreviate)
{
    std::string output;
    output.append(std::to_string(day.tm_mday));
    output.append(" ");
    if (day.tm_mon < 12 && day.tm_mon > -1) output.append(Month_Names[day.tm_mon]);
    output.append(", ");
    JTime empty_end = {-1, -1};
    if (end == empty_end) output.append(to_string(begin));
    else output.append(to_string(begin, end));

    if (day.tm_wday < 7 && day.tm_wday > -1) 
    {
        output.append(", ");
        if (abbreviate) output.append(Day_Names_Abbreviated[day.tm_wday]);
        if (!abbreviate) output.append(Day_Names[day.tm_wday]);
    }
    return output;
}

bool j_button_selectable(const char* label, bool selected)
{
    if (selected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
    }
    bool output = ImGui::Button(label);
    if (selected)
    {
        ImGui::PopStyleColor(3);
    }
    return output;
}

bool j_input_time(std::string label, JTime& time)
{
    if (ImGui::BeginTable(label.c_str(), 3, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
    {
        ImGui::TableNextColumn();
        std::string hours_name = generate_label(label, { 0 });
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("WWWW").x);
        if (ImGui::InputInt(hours_name.c_str(), &(time.hours), 0, 0, ImGuiInputTextFlags_None))
        {
            if (time.hours > 23) time.hours = 23;
            if (time.hours < 0 ) time.hours = 0;
        }
        ImGui::TableNextColumn();
        ImGui::Text(":");
        ImGui::TableNextColumn();
        std::string minutes_name = generate_label(label, { 1 });
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("WWWW").x);
        if (ImGui::InputInt(minutes_name.c_str(), &(time.minutes), 0, 0, ImGuiInputTextFlags_None))
        {
            if (time.minutes > 59) time.minutes = 59;
            if (time.minutes < 0 ) time.minutes = 0;
        }
        ImGui::EndTable();
    }
    return true;
}

std::string generate_label(const std::string prefix, std::vector<int> unique)
{
    std::string output = prefix;
    for (int i = 0; i < unique.size(); i++)
    {
        output.append("." + std::to_string(unique[i]));
    }
    return output;
}

bool j_attendance_combo(const char* label, int* status)
{
    int dummy = 0;
    const char* items[] = { " ", "+", "Б", "-" };
    if (*status == STATUS_WORKED_OUT)
    {
        ImGui::BeginDisabled();
        if (ImGui::BeginCombo("##OTR", "OTP", ImGuiComboFlags_NoArrowButton)) ImGui::EndCombo();
        ImGui::EndDisabled();
        return false;
    }
    if (*status == STATUS_NOT_AWAITED)
    {
        ImGui::BeginDisabled();
        if (ImGui::BeginCombo("##NAW", "...", ImGuiComboFlags_NoArrowButton)) ImGui::EndCombo();
        ImGui::EndDisabled();
        return false;
    }
    const char* combo_preview_value = items[*status];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo(label, combo_preview_value))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (*status == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                *status = n;
                ImGui::EndCombo();
                return true;
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return false;
}