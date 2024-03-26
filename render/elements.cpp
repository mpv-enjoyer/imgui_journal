#include "render.h"

Elements::Picker::Picker(std::vector<std::string> descriptions, std::vector<int> id_list) 
: _descriptions(descriptions), _id_list(id_list)
{
    if (id_list.size() != 0) use_id_list = true;
    if (id_list.size() != descriptions.size())
        throw std::invalid_argument("id list size != descriptions size");
}

int Elements::Picker::show()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
    filter.Draw("Поиск по ученикам");
    ImGui::PopStyleColor(1);
    ImGui::BeginChild("Child window", ImVec2(500,300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < _descriptions.size(); i++)
    {
        if (!filter.PassFilter(_descriptions[i].c_str())) continue;
        
        if (current == i)
        {
            std::string student_button_name = generate_label("Выбран.##", { i });
            j_button_selectable(student_button_name.c_str(), true);
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
    ImGui::EndChild();
    if (use_id_list) return _id_list[current];
    else return current;
}

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

int validate_time_int(std::string input_string, int upper_limit)
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

void Elements::table(int merged_lesson_id)
{
    const Lesson_Info& merged_lesson = PTRREF(Journal::lesson_info(Graphical::wday(), merged_lesson_id));
    if (!Graphical::is_edit_mode() && merged_lesson.is_discontinued()) return;
    bool disabled = merged_lesson.is_discontinued();
    if (disabled) ImGui::BeginDisabled();
    if (merged_lesson_id != 0 && Journal::lesson_info(Graphical::wday(), merged_lesson_id - 1)->get_lesson_pair(0).time_begin == merged_lesson.get_lesson_pair(0).time_begin)
    {
        ImGui::SameLine();
    }
    
    current_lesson.merged_lesson_id = current_merged_lesson;
    ImGui::BeginGroup();
    if (disabled)
    {
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Удаленная"); ImGui::SameLine();
    }
    ImGui::Text(current_merged_lesson_ref.get_description().c_str());
    std::string table_name = generate_label("##table", { current_merged_lesson, (int)all_lessons[current_day_of_the_week].size() });
    std::vector<std::string> current_lesson_names;
    std::vector<int> current_lesson_name_ids;
    for (int current_internal_lesson = 0; current_internal_lesson < current_merged_lesson_ref.get_lessons_size(); current_internal_lesson++)
    {
        current_lesson_names.push_back(Lesson_Names[current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id] + "##" + std::to_string(current_internal_lesson + 1));
        current_lesson_name_ids.push_back(current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id);
    }
    if (ImGui::BeginTable(table_name.c_str(), DEFAULT_COLUMN_COUNT+count_visible_days+1, 
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoPadInnerX,
    ImVec2(std::numeric_limits<float>::max(),(0.0F))))
    {
        ImU32 row_bg_color = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
        ImGui::TableNextColumn(); ImGui::Text("No");
        ImGui::TableNextColumn(); ImGui::Text("ФИ ученика");
        ImGui::TableNextColumn(); ImGui::Text("Д-р");
        ImGui::TableNextColumn(); ImGui::Text("Программа");
        ImGui::TableNextColumn(); ImGui::Text("Цена");
}