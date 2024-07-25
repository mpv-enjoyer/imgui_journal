#include "graphical.h"

Graphical::Graphical(Journal& main_journal) : journal(main_journal)
{
    _edit_mode = true;
    _wday = journal.current_time.tm_wday;
    _visible_days = journal.enumerate_days(_wday);
}

void Graphical::select_wday(int wday)
{
    IM_ASSERT(wday >= 0 && wday < 7);
    _wday = wday;
    _visible_days = journal.enumerate_days(_wday);
}

bool Graphical::attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name)
{
    std::string lesson_concat = to_string({ first_lesson_name, second_lesson_name}, "+").c_str();
    const char* items[] = {lesson_concat.c_str(), first_lesson_name.c_str(), second_lesson_name.c_str()};
    const char* combo_preview_value = items[*attend_data];  // Pass in the preview value visible before opening the combo (it could be anything)
    //ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
    if (ImGui::BeginCombo(label.c_str(), combo_preview_value, ImGuiComboFlags_WidthFitPreview))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (*attend_data == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                *attend_data = n;
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

bool Graphical::button_selectable(const char* label, bool selected, bool small)
{
    if (selected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
    }
    bool output;
    if (!small) output = ImGui::Button(label);
    else output = ImGui::SmallButton(label);
    if (selected)
    {
        ImGui::PopStyleColor(3);
    }
    return output;
}

bool Graphical::button_dangerous(const char* label)
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.8f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.9f, 0.9f));
    bool output = ImGui::Button(label);
    ImGui::PopStyleColor(3);
    return output;
}

bool Graphical::input_time(std::string label, JTime& time)
{
    std::string pre_input_buffer;
    char input_buffer[3];
    if (ImGui::BeginTable(label.c_str(), 3, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
    {
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
        pre_input_buffer = std::to_string(time.hours); if (pre_input_buffer.size()==1) pre_input_buffer = "0" + pre_input_buffer;
        strcpy(input_buffer, pre_input_buffer.c_str());
        std::string hours_name = generate_label(label, { 0 });
        if (ImGui::InputText(hours_name.c_str(), input_buffer, IM_ARRAYSIZE(input_buffer)))
        {
            int new_hours = validate_time_int(std::string(input_buffer), 24);
            if (new_hours != -1) time.hours = new_hours;
        }
        ImGui::TableNextColumn();
        ImGui::Text(":");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
        pre_input_buffer = std::to_string(time.minutes); if (pre_input_buffer.size()==1) pre_input_buffer = "0" + pre_input_buffer;
        strcpy(input_buffer, pre_input_buffer.c_str());
        std::string minutes_name = generate_label(label, { 1 });
        if (ImGui::InputText(minutes_name.c_str(), input_buffer, IM_ARRAYSIZE(input_buffer)))
        {
            int new_minutes = validate_time_int(std::string(input_buffer), 60);
            if (new_minutes != -1) time.minutes = new_minutes;
        }
        ImGui::EndTable();
    }
    return true;
}

bool Graphical::button_colored(const char* label, float r, float g, float b)
{
    ImVec4 color(r, g, b, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    bool result = ImGui::Button(label);
    ImGui::PopStyleColor();
    return result;
}