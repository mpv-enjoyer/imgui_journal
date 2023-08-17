#include "main.h"

bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups)
{

    ImGui::Begin("Список всех учеников", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    static bool add_student = false;
    static Student* new_student;

    if (ImGui::Button("Добавить ученика") || add_student)
    {
        if (add_student == false)
        {
            new_student = new Student();
        }
        bool ignore = false;
        bool result = popup_add_student_to_base(new_student, &ignore, !add_student);
        add_student = true;
        if (result)
        {
            add_student = false;
            if (!ignore) all_students->push_back(*new_student);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::Text("Список всех учеников");
    if (ImGui::BeginTable("students", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("ID[dbg]");
        ImGui::TableSetupColumn("Фамилия и имя", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("No договора");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableSetupColumn("Дата рождения");
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
            ImGui::TableNextColumn(); ImGui::Text(all_students->at(student_id).get_birth_date_string().c_str());
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}

