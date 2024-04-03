#include "add_student_to_group.h"

#define Journal graphical.journal

Popup_Add_Student_To_Group::Popup_Add_Student_To_Group(const Lesson_Info& current_lesson, int merged_lesson_known_id, int wday) 
: current_group(current_lesson.get_group()), merged_lesson_known_id(merged_lesson_known_id), current_lesson(current_lesson)
{
    current_wday = wday;
    for (int i = 0; i < journal->student_count(); i++)
    {
        if (journal->student(i)->is_removed()) continue;
        if (current_group.is_in_group(PTRREF(journal->student(i)))) continue;
        possible_student_descriptions.push_back((journal->student(i)->get_name() + " (" + std::to_string(journal->student(i)->get_contract()) + ")"));
        possible_student_ids.push_back(i);
    }
};

bool Popup_Add_Student_To_Group::show_frame()
{
    POPUP_INIT_FRAME("Добавление ученика в группу")
    {
        if (possible_student_ids.size() == 0) POPUP_CANCEL;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        text_filter.Draw("Поиск по ученикам");
        ImGui::PopStyleColor(1);
        bool select_visible = false;
        ImGui::BeginChild("Child window", ImVec2(0,400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (int i = 0; i < possible_student_descriptions.size(); i++)
        {
            if (!text_filter.PassFilter(possible_student_descriptions[i].c_str())) continue;
            if (current_selected_student == possible_student_ids[i])
            {
                std::string next_button_name = generate_label("Выбран.##", { i });
                j_button_selectable(next_button_name.c_str(), true);
                select_visible = true;
            }
            else
            {
                std::string next_button_name = generate_label("Выбрать##", { i });
                if (ImGui::Button(next_button_name.c_str())) current_selected_student = possible_student_ids[i];
            }
            ImGui::SameLine();
            ImGui::Text(possible_student_descriptions[i].c_str());
        }
        ImGui::EndChild();
        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("OK") && is_ok_possible(select_visible)) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

//only works for 1 lesson = 1 group
void Popup_Add_Student_To_Group::accept_changes()
{
    journal->add_student_to_group(current_selected_student, current_wday, get_merged_lesson_known_id());
}