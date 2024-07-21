#include "helpers.h"

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

std::string to_string(const std::vector<std::string>& strings, std::string separator, const std::vector<bool>& show_only)
{
    std::string output = "";
    bool pick_individual = show_only.size() == strings.size();
    for (int i = 0; i < strings.size(); i++)
    {
        if ((pick_individual && show_only[i]) || !pick_individual) 
        {
            auto index = strings[i].find("##");
            if (index != strings[i].npos)
            {
                std::string temp = strings[i].substr(0, index);
                output.append(temp);
            }
            else output.append(strings[i]);
        }
        if (i != strings.size() - 1) output.append(separator);
    }
    return output;
}

bool j_button_selectable(const char* label, bool selected, bool small)
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

bool j_button_dangerous(const char* label)
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

bool j_input_time(std::string label, JTime& time)
{
    bool result = false;
    std::string pre_input_buffer;
    char input_buffer[3];
    if (ImGui::BeginTable(label.c_str(), 3, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
        pre_input_buffer = std::to_string(time.hours); if (pre_input_buffer.size()==1) pre_input_buffer = "0" + pre_input_buffer;
        strncpy(input_buffer, pre_input_buffer.c_str(), 3);
        IM_ASSERT(input_buffer[2] == '\0' || input_buffer[1] == '\0' || input_buffer[0] == '\0');
        std::string hours_name = generate_label(label, { 0 });
        if (ImGui::InputText(hours_name.c_str(), input_buffer, IM_ARRAYSIZE(input_buffer)))
        {
            int new_hours = validate_time_int(std::string(input_buffer), 24);
            if (new_hours != -1) 
            {
                time.hours = new_hours;
                result = true;
            }
        }
        ImGui::TableNextColumn();
        ImGui::Text(":");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
        pre_input_buffer = std::to_string(time.minutes); if (pre_input_buffer.size()==1) pre_input_buffer = "0" + pre_input_buffer;
        strncpy(input_buffer, pre_input_buffer.c_str(), 3);
        IM_ASSERT(input_buffer[2] == '\0' || input_buffer[1] == '\0' || input_buffer[0] == '\0');
        std::string minutes_name = generate_label(label, { 1 });
        if (ImGui::InputText(minutes_name.c_str(), input_buffer, IM_ARRAYSIZE(input_buffer)))
        {
            int new_minutes = validate_time_int(std::string(input_buffer), 60);
            if (new_minutes != -1)
            {
                time.minutes = new_minutes;
                result = true;
            }
        }
        ImGui::EndTable();
    }
    return result;
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

bool j_age_group_combo(const char* label, int* age_group)
{
    const char* combo_preview_value = Age_Group_Names[*age_group].c_str(); 
    if (ImGui::BeginCombo(label, combo_preview_value, ImGuiComboFlags_WidthFitPreview | ImGuiComboFlags_HeightLargest))
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
};

bool j_attendance_combo(const char* label, int* status, std::string tooltip)
{
    ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
    int dummy = 0;
    const char* items[] = { " ", "V", "Б", "O", "ОТР" };
    //if (*status == STATUS_WORKED_OUT)
    //{
        //ImVec2 gradient_size = ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight());
        //ImGui::InvisibleButton(label, gradient_size);
        //ImGui::BeginDisabled();
        //if (ImGui::BeginCombo("##OTR", "OTP", ImGuiComboFlags_NoArrowButton)) ImGui::EndCombo();
        //ImGui::EndDisabled();
        
        //return false;
    //}
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
};

bool j_attend_data(std::string label, Attend_Data* attend_data, std::string first_lesson_name, std::string second_lesson_name)
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

template <typename T = int, const Student&>
bool is_in_vector(std::vector<T> vector, T to_find)
{
    for (int i = 0; i < vector.size(); i++)
    {
        if (to_find == vector[i]) return true;
    }
    return false;
};

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
};

int get_number_of_days(int month, int year_starting_from_zero)
{
    month++;
    int year = year_starting_from_zero;
    // leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    // months which have 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

int get_first_mwday(int month, int year)
{
    return get_wday(0, month, year);
}

int get_first_wday(int month, int year, int wday)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      1, month, year}; // 1-based day, 0-based month, year since 1900

  std::time_t time_temp = std::mktime(&time_in);

  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm time_out = *std::localtime(&time_temp);
  int first_mday_wday = time_out.tm_wday;
  int diff = ( ( wday - first_mday_wday ) + 7 ) % 7;
  return diff + 1;
}

int get_wday(int day, int month, int year)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      day + 1, month, year }; // 1-based day, 0-based month, year since 1900
  std::time_t time_temp = std::mktime(&time_in);
  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm * time_out = std::localtime(&time_temp);
  return time_out->tm_wday;
}

int get_mday_index_for_wday(int mday, int wday, int month, int year)
{
    int day = get_first_wday(month, year, wday) - MDAY_DIFF;
    int day_count = get_number_of_days(month, year + 1900);
    for (int i = 0; day <= day_count; day += 7, i++)
    {
        if (day == mday) return i;
    }
    return -1;
};

int get_wday_count_in_month(int wday, int month, int year)
{
    int output = 0;
    int day = get_first_wday(month, year, wday);
    int day_count = get_number_of_days(month, year);
    for ( ; day <= day_count; day+=7, output++) { };
    return output;
}

void next_month_for(int& month, int& year)
{
    if (month == 11)
    {
        month = 0;
        year = year + 1;
    }
    else
    {
        month += 1;
    }
}

void previous_month_for(int& month, int& year)
{
    if (month == 0)
    {
        month = 11;
        year = year - 1;
    }
    else
    {
        month -= 1;
    }
}

Time_Archiver::Time_Archiver(std::tm tm)
{
    mday = tm.tm_mday;
    month = tm.tm_mon;
    wday = tm.tm_wday;
    year = tm.tm_yday;
}

std::tm Time_Archiver::to_tm()
{
    std::tm tm;
    tm.tm_mday = mday;
    tm.tm_mon = month;
    tm.tm_wday = wday;
    tm.tm_yday = year;
    return tm;
}
