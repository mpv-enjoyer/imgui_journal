#include "lessons_list.h"

Subwindow_Lessons_List::Subwindow_Lessons_List(std::vector<std::vector<Lesson_Info*>>& all_lessons, std::vector<Calendar_Day*>& all_days, int month, int year) : all_lessons(all_lessons), all_days(all_days)
{ 
    month_num = month;
    year_num = year;
};

bool Subwindow_Lessons_List::show_frame()
{
    ImGui::Begin("Список всех групп", nullptr, WINDOW_FLAGS);
    if (popup_edit_lesson)
    {
        bool result = popup_edit_lesson->show_frame();
        if (result && popup_edit_lesson->check_ok())
        {
            popup_edit_lesson->accept_changes(all_lessons, all_days, month_num, year_num);
        }
        if (result)
        {
            free(popup_edit_lesson);
            popup_edit_lesson = nullptr;
        }
    }
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
                std::string label = generate_label("Изменить группу##", {current_merged_lesson});
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