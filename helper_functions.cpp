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

std::string generate_label(const std::string prefix, std::vector<int> unique)
{
    std::string output = prefix;
    for (int i = 0; i < unique.size(); i++)
    {
        output.append("." + std::to_string(unique[i]));
    }
    return output;
}