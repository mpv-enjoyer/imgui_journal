#include "add_merged_lesson_to_journal.h"

Popup_Add_Merged_Lesson_To_Journal::Popup_Add_Merged_Lesson_To_Journal(int current_day_of_the_week) 
: day_of_the_week(current_day_of_the_week)
{
    lesson_pairs = std::vector<Lesson_Pair>(2, {0,0,0});
}

bool Popup_Add_Merged_Lesson_To_Journal::show_frame()
{
    POPUP_INIT_FRAME(("Добавить группу на " + Day_Names[day_of_the_week]).c_str())
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
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
        ImGui::SeparatorText("Данные новой группы");
        if (ImGui::InputInt("Номер", &group_number))
        {
            if (group_number < 0) group_number = 0;
        };
        ImGui::InputText("Описание", &group_comment);
        ImGui::PopStyleColor();

        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK; 
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

bool Popup_Add_Merged_Lesson_To_Journal::is_ok_possible()
{
    for (int i = 0; i < Journal::lesson_info_count(day_of_the_week); i++)
    {
        if (Journal::lesson_info(day_of_the_week, i)->get_group().get_number() == group_number)
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

    Journal::add_merged_lesson(day_of_the_week, group_number, group_comment, AGEGROUP, lesson_pairs);
}