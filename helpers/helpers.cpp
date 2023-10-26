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
    std::string pre_input_buffer;
    char input_buffer[3];
    if (ImGui::BeginTable(label.c_str(), 3, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
    {
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
        pre_input_buffer = std::to_string(time.hours); if (pre_input_buffer.size()==1) pre_input_buffer = "0" + pre_input_buffer;
        strcpy(input_buffer, pre_input_buffer.c_str());
        //TODO: I don't know whether this strcpy command is safe or not, I've heard complains.
        //      This might lead to unknown bugs
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
        //TODO: I don't know whether this strcpy command is safe or not, I've heard complains.
        //      This might lead to unknown bugs
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
    ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
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
        ImVec2 gradient_size = ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight());
        {
            ImVec2 p0 = ImGui::GetCursorScreenPos();
            ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
            ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
            ImU32 col_b = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(p0, p1, IM_COL32(80, 80, 80, 255));
            //draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
            //ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(100, 0, 0, 100));
            ImGui::InvisibleButton("##gradient1", gradient_size);
            //ImGui::PopStyleColor();
        }
        //ImGui::BeginDisabled();
        //if (ImGui::BeginCombo("##NAW", "...", ImGuiComboFlags_NoArrowButton)) ImGui::EndCombo();
        //ImGui::EndDisabled();
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
};

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

int get_number_of_days(int month, int year)
{
    month++;
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

int calculate_first_mwday(int current_mday, int current_wday)
{
    int first_same = ((current_mday - 1) % 7) + 1;
    int diff = first_same - 1;
    return (current_wday - diff + 7) % 7;
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