#include "lessons_list.h"

Subwindow_Lessons_List::Subwindow_Lessons_List(Graphical* _graphical)
{ 
    graphical = _graphical;
    journal = &(graphical->journal);
};

bool Subwindow_Lessons_List::show_frame()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Список всех групп", nullptr, WINDOW_FLAGS);
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Добавить группу"))
    {
        graphical->popup_add_merged_lesson_to_journal = new Popup_Add_Merged_Lesson_To_Journal(graphical);
    }
    ImGui::Text("Список всех групп");
    if (ImGui::BeginTable("##Список групп", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX))
    {
        ImGui::TableSetupColumn("День недели");
        ImGui::TableSetupColumn("Группа");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        for (int _wday = 0; _wday < 7; _wday++)
        {
            int wday = CONVERT_TO_RU_CALENDAR(_wday);
            for (int merged_lesson_id = 0; merged_lesson_id < journal->lesson_info_count(wday); merged_lesson_id++)
            {
                const auto& current_lesson_info = journal->lesson_info(wday, merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
                bool is_removed_input_buffer = current_lesson_info->is_discontinued();
                if (is_removed_input_buffer) continue;
                ImGui::TableNextRow(); 
                ImGui::TableSetColumnIndex(0); ImGui::Text(Day_Names[wday].c_str());
                ImGui::TableSetColumnIndex(1);
                std::string comment_input_buffer = current_group.get_comment();
                std::string comment_label = generate_label("##input_comment", {wday, merged_lesson_id});
                if (ImGui::InputText(comment_label.c_str(), &comment_input_buffer))
                {
                    journal->set_group_comment(wday, merged_lesson_id, comment_input_buffer);
                }
                int number_input_buffer = current_group.get_number();
                std::string number_label = generate_label("##input_number", {wday, merged_lesson_id});
                if (ImGui::InputInt(number_label.c_str(), &number_input_buffer))
                {
                    journal->set_group_number(wday, merged_lesson_id, number_input_buffer);
                }
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button("Удалить группу"))
                {
                    journal->remove_lesson(wday, merged_lesson_id);
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}