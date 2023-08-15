#include "main.h"

bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups)
{
    ImGui::Begin("asf", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("Список всех учеников");
    
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    if (ImGui::BeginTable("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("ID[dbg]");
        ImGui::TableSetupColumn("Фамилия и имя", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("No договора");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableHeadersRow();
        for (int student_id = 0; student_id < all_students->size(); student_id++)
        {
            ImGui::TableNextColumn(); ImGui::Text(std::to_string(student_id).c_str());
            ImGui::TableNextColumn(); ImGui::Text(all_students->at(student_id).get_name().c_str());
            ImGui::TableNextColumn(); ImGui::Text(std::to_string(all_students->at(student_id).get_contract()).c_str());
            ImGui::TableNextColumn();
            for (int group_id = 0; group_id < all_groups->size(); group_id++) //TODO: literally doing twice as much sork.
            {
                if (all_groups->at(group_id).is_in_group(student_id)) 
                {
                    ImGui::Text(std::to_string(group_id).c_str()); ImGui::SameLine();
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}

