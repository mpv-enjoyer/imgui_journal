#include "render.h"

bool Elements::button_selectable(const char* label, bool selected, bool small)
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

bool Elements::button_dangerous(const char* label)
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.8f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f / 7.0f, 0.9f, 0.9f));
    bool output = ImGui::Button(label);
    ImGui::PopStyleColor(3);
    return output;
}

int input_validate_time_int(std::string input_string, int upper_limit)
{
    if (input_string.size() == 0) return -1;
    for (int i = 0; i < input_string.size(); i++)
    {
        if (input_string[i] < '0' || input_string[i] > '9') return -1;
    }
    int output = std::stoi(input_string);
    if (output >= upper_limit) return upper_limit - 1;
    if (output < 0) return 0;
    return output;
}

bool Elements::input_time(std::string label, JTime& time)
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
            int new_hours = input_validate_time_int(std::string(input_buffer), 24);
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
            int new_minutes = input_validate_time_int(std::string(input_buffer), 60);
            if (new_minutes != -1) time.minutes = new_minutes;
        }
        ImGui::EndTable();
    }
    return true;
}

bool Elements::attendance_combo(const char* label, int* status, std::string tooltip)
{
    ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
    int dummy = 0;
    const char* items[] = { " ", "V", "Б", "O", "ОТР" };
    if (*status == STATUS_NOT_AWAITED)
    {
        ImVec2 gradient_size = ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight());
        {
            ImVec2 p0 = ImGui::GetCursorScreenPos();
            ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(p0, p1, IM_COL32(135, 135, 135, 255));
            ImGui::InvisibleButton("##gradient1", gradient_size);
        }
        return false;
    }
    const char* combo_preview_value = items[*status];  // Pass in the preview value visible before opening the combo (it could be anything)
    bool modify_for_workout = *status == STATUS_WORKED_OUT;
    ImGuiComboFlags flags = modify_for_workout ? ImGuiComboFlags_NoArrowButton : 0; 
    if (ImGui::BeginCombo(label, combo_preview_value, flags))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items) - (!modify_for_workout); n++)
        {
            const bool is_selected = (*status == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                *status = n;
                ImGui::EndCombo();
                return true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (*status == STATUS_WORKED_OUT) ImGui::SetItemTooltip(tooltip.c_str());
    return false;
};

bool Elements::attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name)
{
    std::string lesson_concat = to_string({ first_lesson_name, second_lesson_name}, "+").c_str();
    const char* items[] = {lesson_concat.c_str(), first_lesson_name.c_str(), second_lesson_name.c_str()};
    const char* combo_preview_value = items[*attend_data];
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
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return false;
}
