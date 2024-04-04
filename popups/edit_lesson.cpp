#include "edit_lesson.h"

Popup_Edit_Lesson::Popup_Edit_Lesson(Graphical* _graphical, int wday, int merged_lesson_id)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    lesson_info = journal->lesson_info(wday, merged_lesson_id);
    this->wday = wday;
    this->merged_lesson_id = merged_lesson_id;
    first_lesson_pair = lesson_info->get_lesson_pair(0);
    if (lesson_info->get_lessons_size() == 2)
    {
        second_lesson_pair = lesson_info->get_lesson_pair(1);
        second_lesson_exists = true;
    }
    group_description = lesson_info->get_group().get_comment();
    group_number = lesson_info->get_group().get_number();
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

void Popup_Edit_Lesson::accept_changes()
{
    std::vector<Lesson_Pair> input_pairs = { first_lesson_pair };
    if (second_lesson_exists) input_pairs.push_back(second_lesson_pair);
    journal->edit_lesson(wday, merged_lesson_id, group_number, group_description, input_pairs);
}