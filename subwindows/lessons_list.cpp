#include "lessons_list.h"

Subwindow_Lessons_List::Subwindow_Lessons_List(Graphical* _graphical)
{ 
    graphical = _graphical;
    journal = &(graphical->journal);
};

bool Subwindow_Lessons_List::show_frame()
{
    bool edit_mode = graphical->edit_mode;
    bool is_current = journal->get_state() == Journal::State::Fullaccess;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, background);
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Список всех групп", nullptr, WINDOW_FLAGS);
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::PopStyleColor();
        ImGui::End();
        return true;
    }
    ImGui::SameLine();
    if (is_current)
    {
        if (ImGui::Button("Добавить группу"))
        {
            graphical->popup_add_merged_lesson_to_journal = new Popup_Add_Merged_Lesson_To_Journal(graphical);
        }
    }
    else
    {
        ImVec4 red = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
        ImGui::TextColored(red, "Вы смотрите данные другого месяца (%s). Редактирование доступно только для текущего месяца.", Month_Names[journal->current_month()].c_str());
    }

    ImGui::BeginChild("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("Список всех групп");
    if (ImGui::BeginTable("##Список групп", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX))
    {
        ImGui::TableSetupColumn("Номер группы");
        ImGui::TableSetupColumn("День недели");
        ImGui::TableSetupColumn("Время посещения");
        ImGui::TableSetupColumn("Описание группы");
        ImGui::TableSetupColumn("Возраст");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        for (int _wday = 0; _wday < 7; _wday++)
        {
            int wday = CONVERT_TO_RU_CALENDAR(_wday);
            for (int merged_lesson_id = 0; merged_lesson_id < journal->lesson_info_count(wday); merged_lesson_id++)
            {
                const auto& current_lesson_info = journal->lesson_info(wday, merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
                bool is_removed_input_buffer = current_lesson_info->is_removed();
                if (is_removed_input_buffer && !edit_mode) continue;
                if (is_removed_input_buffer) ImGui::BeginDisabled();
                ImGui::TableNextRow(); 
                ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%i", current_group.get_number());
                ImGui::TableSetColumnIndex(1);
                    ImGui::Text(Day_Names[wday].c_str());
                ImGui::TableSetColumnIndex(2);
                    std::vector<InternalLessonInfo> pairs_buffer = current_lesson_info->get_lesson_pairs();
                    for (int i = 0; i < pairs_buffer.size(); i++)
                    {
                        ImGui::Text("%i. %s", i + 1, Lesson_Names[pairs_buffer[i].lesson_name_id].c_str());
                        ImGui::Text(to_string(pairs_buffer[i].time_begin, pairs_buffer[i].time_end).c_str());
                    }
                ImGui::TableSetColumnIndex(3);
                    ImGui::Text(current_group.get_comment().c_str());
                ImGui::TableSetColumnIndex(4);
                    ImGui::Text(Age_Group_Names[current_group.get_age_group()].c_str());
                ImGui::TableSetColumnIndex(5);
                if (is_removed_input_buffer) ImGui::EndDisabled();

                if (!is_current) ImGui::BeginDisabled();

                std::string restore_label = generate_label("Восстановить группу##", {wday, merged_lesson_id});
                std::string delete_label = generate_label("Удалить группу##", {wday, merged_lesson_id});
                std::string edit_label = generate_label("Изменить группу##", {wday, merged_lesson_id});
                if (edit_mode)
                {
                    if (is_removed_input_buffer)
                    {
                        if (j_button_colored(restore_label.c_str(), 0.1, 0.9, 0.1)) journal->restore_lesson(wday, merged_lesson_id);
                    }
                    else
                    {
                        if (j_button_dangerous(delete_label.c_str()))
                        {
                            graphical->popup_confirm_delete_lesson = new Popup_Confirm_Delete_Lesson(graphical, wday, merged_lesson_id);
                        }
                    }
                }
                else if (j_button_dangerous(delete_label.c_str()))
                {
                    graphical->popup_confirm_delete_lesson = new Popup_Confirm_Delete_Lesson(graphical, wday, merged_lesson_id);
                }
                if (!is_removed_input_buffer && ImGui::Button(edit_label.c_str()))
                {
                    graphical->popup_add_merged_lesson_to_journal = new Popup_Add_Merged_Lesson_To_Journal(graphical, merged_lesson_id, wday);
                }

                if (!is_current) ImGui::EndDisabled();
            }
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleColor(); // BG
    return false;
}