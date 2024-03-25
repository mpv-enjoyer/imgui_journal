#include "lessons_list.h"

Subwindow_Lessons_List::Subwindow_Lessons_List()
{ 

};

bool Subwindow_Lessons_List::show_frame()
{
    ImGui::Begin("Список всех групп", nullptr, WINDOW_FLAGS);
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
    ImGui::Checkbox("Режим редактирования", &edit_mode);
    ImGui::PopStyleColor();
    ImGui::Text("Список всех групп");
    if (ImGui::BeginTable("##Список групп", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("День недели");
        ImGui::TableSetupColumn("Группа");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        for (int _wday = 0; _wday < 7; _wday++)
        {
            int wday = CONVERT_TO_RU_CALENDAR(_wday);
            for (int merged_lesson_id = 0; merged_lesson_id < Journal::lesson_info_count(wday); merged_lesson_id++)
            {
                const auto& current_lesson_info = Journal::lesson_info(wday, merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
                bool is_removed_input_buffer = current_lesson_info->is_discontinued();
                if (is_removed_input_buffer) continue;
                ImGui::TableNextRow(); 
                ImGui::TableSetColumnIndex(0); ImGui::Text(Day_Names[wday].c_str());
                ImGui::TableSetColumnIndex(1);
                std::string comment_input_buffer = current_group.get_comment();
                std::string comment_label = generate_label("input_comment", {wday, merged_lesson_id});
                if (ImGui::InputText(comment_label.c_str(), &comment_input_buffer))
                {
                    Journal::set_group_comment(wday, merged_lesson_id, comment_input_buffer);
                }
                int number_input_buffer = current_group.get_number();
                std::string number_label = generate_label("input_number", {wday, merged_lesson_id});
                if (ImGui::InputInt(number_label.c_str(), &number_input_buffer))
                {
                    Journal::set_group_number(wday, merged_lesson_id, number_input_buffer);
                }
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button("Удалить группу"))
                {
                    Journal::TODO
                }
            }
        }

        for (int row = 0; row < 7; row++)
        {
            int current_day_ru = row == 6 ? 0 : row + 1;
            int current_day_lessons_size = all_lessons[current_day_ru].size();
            if (current_day_lessons_size == 0) continue;
            for (int current_merged_lesson = 0; current_merged_lesson < current_day_lessons_size; current_merged_lesson++)
            {
                Lesson_Info* current_lesson_info = all_lessons[current_day_ru][current_merged_lesson];
                bool is_removed_input_buffer = current_lesson_info->is_discontinued();
                if (is_removed_input_buffer && !edit_mode) continue;
                ImGui::TableNextRow(); 
                ImGui::TableSetColumnIndex(0); ImGui::Text(Day_Names[current_day_ru].c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text(current_lesson_info->get_description().c_str());
                ImGui::TableSetColumnIndex(2);
                std::string label = generate_label("Изменить группу##", {current_merged_lesson, current_day_ru});
                if (ImGui::Button(label.c_str()))
                {
                    popup_edit_lesson = new Popup_Edit_Lesson(PTRREF(current_lesson_info), current_day_ru, current_merged_lesson);
                };
                if (!edit_mode && !is_removed_input_buffer)
                {
                    if (j_button_dangerous("Удалить группу")) current_lesson_info->discontinue();
                }
                if (edit_mode && ImGui::Checkbox(generate_label("Удалить группу?##", {current_merged_lesson}).c_str(), &is_removed_input_buffer))
                {
                    if (is_removed_input_buffer) current_lesson_info->discontinue();
                    if (!is_removed_input_buffer) current_lesson_info->restore();
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}