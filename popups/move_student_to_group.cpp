#include "move_student_to_group.h"

Popup_Move_Student_To_Group::Popup_Move_Student_To_Group(Graphical *_graphical, const Lesson_Info *current, int current_lesson_info_wday, int current_lesson_info_id, int student_id, bool* callback)
: current_lesson_info_wday(current_lesson_info_wday), current_lesson_info_id(current_lesson_info_id), student_id(student_id), callback(callback)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    current_lesson_info = current;
}

bool Popup_Move_Student_To_Group::show_frame()
{
    if (begin_frame("Перенос ученика в другую группу"))
    {
        ImGui::Text("Текущая группа: %s, %s", journal->Wday_name_short(current_lesson_info_wday).c_str(), current_lesson_info->get_description().c_str());
        if (ImGui::BeginChild("##child", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            for (int _wday = 0; _wday < 7; _wday++)
            {
                int wday = CONVERT_TO_EN_CALENDAR(_wday);
                for (int merged_lesson_id = 0; merged_lesson_id < journal->lesson_info_count(wday); merged_lesson_id++)
                {
                    if (journal->lesson_info(wday, merged_lesson_id) == current_lesson_info) continue;
                    if (journal->lesson_info(wday, merged_lesson_id)->get_group().find_student(PTRREF(journal->student(student_id))) != -1) continue;
                    std::string label = journal->Wday_name_short(wday) + ", " + journal->lesson_info(wday, merged_lesson_id)->get_description();
                    bool selected = wday == select_wday && merged_lesson_id == select_merged_lesson_id;
                    std::string button_label = generate_label(std::string(selected ? "Выбран" : "Выбрать") + "##", {_wday, merged_lesson_id});
                    if (j_button_selectable(button_label.c_str(), selected))
                    {
                        select_wday = wday;
                        select_merged_lesson_id = merged_lesson_id;
                    }
                    ImGui::SameLine();
                    ImGui::Text(label.c_str());
                }
            }
            ImGui::EndChild();
        }
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Move_Student_To_Group::accept_changes()
{
    journal->move_student_to_group(current_lesson_info_wday, current_lesson_info_id, student_id, select_wday, select_merged_lesson_id);
    *callback = true;
}
