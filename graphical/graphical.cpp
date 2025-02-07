#include "graphical.h"

JournalHolder::JournalHolder(Journal& main_journal) : journal(main_journal)
{
    _edit_mode = false;
    _wday = journal.current_time.tm_wday;
    _visible_days = journal.enumerate_days(_wday);
}

void JournalHolder::select_wday(int wday)
{
    IM_ASSERT(wday >= 0 && wday < 7);
    _wday = wday;
    _visible_days = journal.enumerate_days(_wday);
}

void JournalHolder::set_edit_mode(bool value)
{
    _edit_mode = value;
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

bool Graphical::age_group_combo(const char *label, int *age_group, bool shrink)
{
    const char* combo_preview_value = Age_Group_Names[*age_group].c_str();
    ImGuiComboFlags flags = ImGuiComboFlags_HeightLargest;
    if (shrink) flags |= ImGuiComboFlags_WidthFitPreview;
    if (ImGui::BeginCombo(label, combo_preview_value, flags))
    {
        for (int n = 0; n < Age_Group_Names.size(); n++)
        {
            const bool is_selected = (*age_group == n);
            if (ImGui::Selectable(Age_Group_Names[n].c_str(), is_selected))
            {
                *age_group = n;
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

bool Graphical::attendance_combo(const char *label, int *status, std::string tooltip)
{
    ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
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
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (*status == STATUS_WORKED_OUT) ImGui::SetItemTooltip(tooltip.c_str());
    return false;
}

Graphical::StudentPicker::StudentPicker(std::vector<std::string> descriptions, std::vector<int> id_list) 
: _descriptions(descriptions), _id_list(id_list), use_id_list(id_list.size() != 0)
{
    if (id_list.size() != descriptions.size())
        throw std::invalid_argument("id list size != descriptions size");
}

int Graphical::StudentPicker::show()
{
    Group group;
    //ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
    filter.Draw("Поиск с учётом регистра");
    ImGui::PopStyleColor(1);
    Child child("Child window", ImVec2(500, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    //ImGui::BeginChild("Child window", ImVec2(500, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < _descriptions.size(); i++)
    {
        if (!filter.PassFilter(_descriptions[i].c_str())) continue;
        
        if (current == i)
        {
            std::string student_button_name = generate_label("Выбран.##", { i });
            Graphical::button_selectable(student_button_name.c_str(), true);
        }
        else
        {
            std::string student_button_name = generate_label("Выбрать##", { i });
            if (ImGui::Button(student_button_name.c_str())) 
            {
                current = i;
            }
        }
        ImGui::SameLine();
        ImGui::Text(_descriptions[i].c_str());
    }
    //ImGui::EndChild();
    //ImGui::EndGroup();
    if (use_id_list && is_valid()) return _id_list[current];
    else return current;
}

bool Graphical::StudentPicker::is_valid()
{
    return current != -1;
}
