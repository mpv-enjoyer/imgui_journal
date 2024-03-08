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
    for (int i = 0; i < Journal::all_groups().size(); i++)
    {
        if (Journal::all_groups()[i]->get_number() == group_number && Journal::all_groups()[i]->get_day_of_the_week() == day_of_the_week) 
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
    Group* new_group = new Group();
    Lesson_Info* lesson_info = new Lesson_Info(PTRREF(new_group));
    new_group->set_number(group_number);
    new_group->set_day_of_the_week(day_of_the_week);
    new_group->set_comment(group_comment);
    int current_pair_name_id = combo_lesson_name_id;
    if (combo_lesson_name_id >= 2) current_pair_name_id -= 2;
    lesson_pairs[0].lesson_name_id = current_pair_name_id;
    lesson_info->add_lesson_pair(lesson_pairs[0]);
    if (combo_lesson_name_id == 2)
    {
        lesson_pairs[1].lesson_name_id = 1;
        lesson_info->add_lesson_pair(lesson_pairs[1]);
    } 
    if (combo_lesson_name_id == 3)
    {
        lesson_pairs[1].lesson_name_id = 0;
        lesson_info->add_lesson_pair(lesson_pairs[1]);
    }
    if (combo_lesson_name_id > 3)
    {
        lesson_pairs[1].lesson_name_id = current_pair_name_id;
        lesson_info->add_lesson_pair(lesson_pairs[1]);
    }
    Journal::edit_all_groups().push_back(new_group);
    lesson_info->set_group(PTRREF(new_group));
    std::vector<Lesson_Info*>& lessons_in_this_day = std::ref(Journal::edit_all_lessons()[day_of_the_week]);
    int new_merged_lesson_known_id = lessons_in_this_day.size();
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        if (PTRREF(lessons_in_this_day[i]) > PTRREF(lesson_info))
        {
            new_merged_lesson_known_id = i;
            break;
        }
    }
    lessons_in_this_day.insert(lessons_in_this_day.begin() + new_merged_lesson_known_id, lesson_info);
    //(*all_lessons)[day_of_the_week][(*all_lessons)[day_of_the_week].size() - 1].set_group(all_groups[all_groups.size() - 1]); useless?
    for (int i = 0; i < Journal::visible_days().size(); i++)
    {
        bool await_no_one = false;
        if (!(Journal::visible_days()[i].is_future || Journal::visible_days()[i].is_today)) await_no_one = true;
        Journal::visible_days()[i].day->add_merged_lesson(PTRREF(lesson_info), await_no_one, new_merged_lesson_known_id);
    }
}