#include "add_merged_lesson_to_journal.h"

Popup_Add_Merged_Lesson_To_Journal::Popup_Add_Merged_Lesson_To_Journal(Graphical* _graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    day_of_the_week = CONVERT_TO_EN_CALENDAR(_graphical->wday);
    lesson_pairs = std::vector<Lesson_Pair>(2, {0,0,0});
}

bool Popup_Add_Merged_Lesson_To_Journal::show_frame()
{
    if (begin_frame(("Добавить группу на " + Day_Names[day_of_the_week]).c_str()))
    {
        ImGuiID id = ImGui::GetID(("Добавить группу на " + Day_Names[day_of_the_week]).c_str());
        ImGui::Text(ImGui::GetIO().WantCaptureMouseUnlessPopupClose ? "WantCaptureMouseUnlessPopupClose" : "not WantCaptureMouseUnlessPopupClose");
        ImGui::Text(ImGui::GetIO().NavActive ? "NavActive" : "not NavActive");
        ImGui::Text(ImGui::IsAnyItemActive() ? "Item active" : "Item not active");
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        ImGui::Combo("День недели", &day_of_the_week, "Пн\0Вт\0Ср\0Чт\0Пт\0Сб\0Вс\0\0", 7);
        ImGui::Combo("Программа", &combo_lesson_name_id, "ИЗО\0Лепка\0ИЗО+Лепка\0Лепка+ИЗО\0Дизайн\0Черчение\0Спецкурс\0\0");
        for (int i = 0; i < 2; i++)
        {
            if (combo_lesson_name_id == 2 || combo_lesson_name_id == 3) 
            {
                std::string lesson_name;
                if (i == 0) lesson_name = Lesson_Names[combo_lesson_name_id - 2];
                else lesson_name = Lesson_Names[3 - combo_lesson_name_id]; //mirrored
                ImGui::Text("%i. %s", i + 1, lesson_name.c_str());
            }
            else if (combo_lesson_name_id > 3)
            {
                std::string lesson_name = Lesson_Names[combo_lesson_name_id - 2];
                ImGui::Text("%i. %s", i + 1, lesson_name.c_str());
            }
            else if (i == 1) break; 
            ImGui::PushID(i);
            j_input_time("##LessonBegin", lesson_pairs[i].time_begin);
            ImGui::SameLine();
            ImGui::Text(" _ ");
            ImGui::SameLine();
            j_input_time("##LessonEnd", lesson_pairs[i].time_end);
            ImGui::PopID();
        }
        if (ImGui::InputInt("Номер", &group_number))
        {
            if (group_number < 0) group_number = 0;
        };
        ImGui::InputText("Описание (необязательно)", &group_comment);
        j_age_group_combo("Возраст (необязательно)", &age_group, false);
        ImGui::PopStyleColor();

        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK; 
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

bool Popup_Add_Merged_Lesson_To_Journal::is_ok_possible()
{
    for (int i = 0; i < journal->lesson_info_count(day_of_the_week); i++)
    {
        if (journal->lesson_info(day_of_the_week, i)->get_group().get_number() == group_number)
        {
            error("Такая группа уже существует");
            return false;
        }
    }
    if (combo_lesson_name_id >= 2)
    {
        bool insane_time = false;
        insane_time = insane_time || lesson_pairs[0].time_begin >= lesson_pairs[0].time_end;
        insane_time = insane_time || lesson_pairs[0].time_end > lesson_pairs[1].time_begin;
        insane_time = insane_time || lesson_pairs[1].time_begin >= lesson_pairs[1].time_end;
        if (insane_time) error("Недопустимое время");
        return !insane_time;
    }
    else 
        if (lesson_pairs[0].time_begin >= lesson_pairs[0].time_end) { error("Недопустимое время"); return false; }
    return true;
};

void Popup_Add_Merged_Lesson_To_Journal::accept_changes()
{
    IM_ASSERT(check_ok());
    
    int current_pair_name_id = combo_lesson_name_id;
    if (combo_lesson_name_id >= 2) current_pair_name_id -= 2;
    lesson_pairs[0].lesson_name_id = current_pair_name_id;
    if (combo_lesson_name_id == 2)
    {
        lesson_pairs[1].lesson_name_id = 1;
    } 
    else if (combo_lesson_name_id == 3)
    {
        lesson_pairs[1].lesson_name_id = 0;
    }
    else if (combo_lesson_name_id > 3)
    {
        lesson_pairs[1].lesson_name_id = current_pair_name_id;
    }
    else lesson_pairs.erase(lesson_pairs.begin() + 1);

    day_of_the_week = CONVERT_TO_RU_CALENDAR(day_of_the_week);
    journal->add_merged_lesson(day_of_the_week, group_number, group_comment, age_group, lesson_pairs);
}