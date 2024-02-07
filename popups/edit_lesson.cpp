#include "edit_lesson.h"

Popup_Edit_Lesson::Popup_Edit_Lesson(Lesson_Info& lesson_info, int day, int merged_lesson_id) : lesson_info(lesson_info)
{
    this->day = day;
    this->merged_lesson_id = merged_lesson_id;
    first_lesson_pair = lesson_info.get_lesson_pair(0);
    if (lesson_info.get_lessons_size() == 2)
    {
        second_lesson_pair = lesson_info.get_lesson_pair(1);
        second_lesson_exists = true;
    }
    group_description = lesson_info.get_group().get_comment();
    group_number = lesson_info.get_group().get_number();
}

bool Popup_Edit_Lesson::show_frame()
{
    POPUP_INIT_FRAME("Изменить группу")
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        ImGui::Text(("1." + Lesson_Names[first_lesson_pair.lesson_name_id]).c_str());
        j_input_time("##edit_lesson_first_begin", first_lesson_pair.time_begin); ImGui::SameLine();
        ImGui::Text(" - "); ImGui::SameLine();
        j_input_time("##edit_lesson_first_end", first_lesson_pair.time_end);
        if (second_lesson_exists)
        {
            ImGui::Text(("2." + Lesson_Names[second_lesson_pair.lesson_name_id]).c_str());
            j_input_time("##edit_lesson_second_begin", second_lesson_pair.time_begin); ImGui::SameLine();
            ImGui::Text(" - "); ImGui::SameLine();
            j_input_time("##edit_lesson_second_end", second_lesson_pair.time_end);
        }
        ImGui::InputText("Описание", &group_description);
        if (ImGui::InputInt("Номер", &group_number) && group_number < 0) group_number = 0;
        ImGui::PopStyleColor();
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) POPUP_CANCEL;
        ImGui::SameLine();
        print_error();
    }
    ImGui::EndPopup();
    return false;
}

void Popup_Edit_Lesson::accept_changes(std::vector<std::vector<Lesson_Info*>>& all_lessons, std::vector<Calendar_Day*>& all_days, int current_month, int current_year)
{
    lesson_info.delete_lesson_pair(0); lesson_info.add_lesson_pair(first_lesson_pair);
    if (second_lesson_exists)
    {
        lesson_info.delete_lesson_pair(0); lesson_info.add_lesson_pair(second_lesson_pair);
    }
    lesson_info.get_group().set_number(group_number);
    lesson_info.get_group().set_comment(group_description);
    all_lessons[day].erase(all_lessons[day].begin() + merged_lesson_id);
    int new_index = all_lessons[day].size();
    for (int j = 0; j < all_lessons[day].size(); j++)
    {
        if (PTRREF(all_lessons[day][j]) > lesson_info) 
        {
            new_index = j;
            break;
        }
    }
    all_lessons[day].insert(all_lessons[day].begin() + new_index, &lesson_info);
    int index_input_for_days_update = new_index;
    if (new_index >= merged_lesson_id && new_index != all_lessons[day].size() - 1) new_index++;
    int first_mday = get_first_wday(current_month, current_year, day) - 1;
    int days_num = all_days.size();
    for (int mday = first_mday; mday < days_num; mday += 7)
    {
        all_days[mday]->swap_merged_lessons(merged_lesson_id, index_input_for_days_update);
    }
}