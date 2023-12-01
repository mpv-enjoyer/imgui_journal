#include "lessons_list.h"

Subwindow_Lessons_List::Subwindow_Lessons_List(std::vector<std::vector<Lesson_Info*>>& all_lessons) : all_lessons(all_lessons) { };

bool Subwindow_Lessons_List::show_frame()
{
    ImGui::Begin("Список всех групп", nullptr, WINDOW_FLAGS);
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::Text("Список всех групп");
    if (ImGui::BeginTable("##Список групп", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("День недели");
        ImGui::TableSetupColumn("Урок");
        ImGui::TableSetupColumn("Ученики");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        //ImGui::TableNextRow();
        for (int row = 0; row < 7; row++)
        {
            int current_day_ru = row ? row - 1 : 6;
            int current_day_lessons_size = all_lessons[current_day_ru].size();
            if (current_day_lessons_size == 0) continue;
            for (int current_merged_lesson = 0; current_merged_lesson < current_day_lessons_size; current_merged_lesson++)
            {
                Lesson_Info* current_lesson_info = all_lessons[current_day_ru][current_merged_lesson];
                ImGui::TableNextRow(); 
                ImGui::TableSetColumnIndex(0); ImGui::Text(Day_Names[row].c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text(current_lesson_info->get_description().c_str());
                ImGui::TableSetColumnIndex(2);
                for (int current_student = 0; current_student < current_lesson_info->get_group().get_size(); current_student++)
                {
                    std::string current_text = current_lesson_info->get_group().get_student(current_student).get_name();
                    //ImGui::Text(current_lesson_info->get_group().get_student(current_student).get_name().c_str());
                    current_text += " (" + std::to_string(current_lesson_info->get_group().get_student(current_student).get_contract()) + ")";
                    ImGui::Text(current_text.c_str());
                }
                ImGui::TableSetColumnIndex(3); ImGui::Button("Изменить урок");
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}