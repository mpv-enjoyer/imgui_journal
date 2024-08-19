#include "confirm_delete_lesson.h"

Popup_Confirm_Delete_Lesson::Popup_Confirm_Delete_Lesson(Graphical *_graphical, int wday, int merged_lesson_id)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    _wday = wday;
    _merged_lesson_id = merged_lesson_id;
}

bool Popup_Confirm_Delete_Lesson::show_frame()
{
    const Lesson_Info* lesson_info = journal->lesson_info(_wday, _merged_lesson_id);
    const Group& group = lesson_info->get_group();
    IM_ASSERT(!lesson_info->is_discontinued());
    if (begin_frame("Удалить группу?"))
    {
        ImGui::Text("Номер группы: %i", group.get_number());

        std::vector<Lesson_Pair> pairs_buffer = lesson_info->get_lesson_pairs();
        for (int i = 0; i < pairs_buffer.size(); i++)
        {
            ImGui::Text("%i. %s", i + 1, Lesson_Names[pairs_buffer[i].lesson_name_id].c_str());
            ImGui::Text(to_string(pairs_buffer[i].time_begin, pairs_buffer[i].time_end).c_str());
        }

        ImGui::Text("Описание: %s", group.get_comment().c_str());

        ImGui::Text("Возраст: %s", Age_Group_Names[group.get_age_group()].c_str());
        
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Confirm_Delete_Lesson::accept_changes()
{
    IM_ASSERT(check_ok());
    journal->remove_lesson(_wday, _merged_lesson_id);
}