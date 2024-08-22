#include "add_merged_lesson_to_journal.h"

int get_combo_lesson_name_id(std::vector<Lesson_Pair> pairs)
{
    int first_lesson_id = pairs[0].lesson_name_id;
    int second_lesson_id = pairs.size() > 1 ? pairs[1].lesson_name_id : -1;
    if (first_lesson_id == NAME_DRAWING && second_lesson_id == NAME_SCULPTING) return 2;
    if (first_lesson_id == NAME_SCULPTING && second_lesson_id == NAME_DRAWING) return 3;
    if (first_lesson_id == NAME_DRAWING) return 0;
    if (first_lesson_id == NAME_SCULPTING) return 1;
    if (first_lesson_id == NAME_DESIGN) return 4;
    if (first_lesson_id == NAME_SPECIALCOURSE) return 5;
    IM_ASSERT(false && "invalid pairs");
}

Popup_Add_Merged_Lesson_To_Journal::Popup_Add_Merged_Lesson_To_Journal(Graphical* _graphical, int existing_lesson_info_id, int wday)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    if (existing_lesson_info_id != -1)
    {
        this->existing_lesson_info_id = existing_lesson_info_id;
        day_of_the_week = wday;
        lesson_pairs = journal->lesson_info(wday, existing_lesson_info_id)->get_lesson_pairs();
        combo_lesson_name_id = get_combo_lesson_name_id(lesson_pairs);
        const Group& group = journal->lesson_info(wday, existing_lesson_info_id)->get_group();
        group_number = group.get_number();
        group_comment = group.get_comment();
        age_group = group.get_age_group();
    }
    else
    {
        day_of_the_week = _graphical->wday;
        lesson_pairs = std::vector<Lesson_Pair>(2, {0,0,0});
    }
    
}

bool Popup_Add_Merged_Lesson_To_Journal::show_frame()
{
    const Lesson_Info* existing_lesson_info = existing_lesson_info_id != -1 ? journal->lesson_info(day_of_the_week, existing_lesson_info_id) : nullptr;
    std::string label = existing_lesson_info ? "Изменить группу " + Day_Names[day_of_the_week] : "Добавить группу на " + Day_Names[day_of_the_week];
    if (begin_frame(label.c_str()))
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        if (existing_lesson_info) ImGui::BeginDisabled();
        int combo_day_of_the_week = CONVERT_TO_EN_CALENDAR(day_of_the_week);
        if (ImGui::Combo("День недели", &combo_day_of_the_week, "Пн\0Вт\0Ср\0Чт\0Пт\0Сб\0Вс\0\0", 7))
        {
            day_of_the_week = CONVERT_TO_RU_CALENDAR(combo_day_of_the_week);
        }
        
        ImGui::Combo("Программа", &combo_lesson_name_id, "ИЗО\0Лепка\0ИЗО+Лепка\0Лепка+ИЗО\0Дизайн\0Черчение\0Спецкурс\0\0");
        if (existing_lesson_info) ImGui::EndDisabled();
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
    if (existing_lesson_info_id != -1 && journal->lesson_info(day_of_the_week, existing_lesson_info_id)->get_group().get_number() == group_number)
    {
        /* Existing lesson info didn't change the number */
    }
    else if (journal->does_group_exist(day_of_the_week, group_number))
    {
        error("Группа с таким номером в " + Day_Names[day_of_the_week] + " уже существует");
        return false;
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
    else if (lesson_pairs[0].time_begin >= lesson_pairs[0].time_end) 
    {
        error("Недопустимое время");
        return false;
    }
    return true;
};

void Popup_Add_Merged_Lesson_To_Journal::accept_changes()
{
    IM_ASSERT(check_ok());
    
    if (existing_lesson_info_id != -1)
    {
        journal->set_group_age_group(day_of_the_week, existing_lesson_info_id, age_group);
        journal->set_group_comment(day_of_the_week, existing_lesson_info_id, group_comment);
        journal->set_group_number(day_of_the_week, existing_lesson_info_id, group_number);
        std::vector<Lesson_Pair> loaded_pairs = journal->lesson_info(day_of_the_week, existing_lesson_info_id)->get_lesson_pairs();
        for (int i = 0; i < loaded_pairs.size(); i++)
        {
            loaded_pairs[i].time_begin = lesson_pairs[i].time_begin;
            loaded_pairs[i].time_end = lesson_pairs[i].time_end;
        }
        journal->edit_lesson_pairs(day_of_the_week, existing_lesson_info_id, loaded_pairs);
        return;
    }

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

    journal->add_merged_lesson(day_of_the_week, group_number, group_comment, age_group, lesson_pairs);
}