#include "main.h"
#include "popups.h"
#include "helper_functions.h"

bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups, int* popup_edit_ignore_lessons_is_open)
{
    ImGui::Begin("Список всех учеников", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    static Popup_Add_Student_To_Base* popup_add_student_to_base = nullptr;
    static bool edit_mode = false;
    
    if (popup_add_student_to_base)
    {
        const bool result = popup_add_student_to_base->show_frame();
        if (result && popup_add_student_to_base->check_ok())
        {
            popup_add_student_to_base->accept_changes(*all_students);
        }
        if (result)
        {
            free(popup_add_student_to_base);
            popup_add_student_to_base = nullptr;
        }
    }

    if (ImGui::Button("Добавить ученика##в общий список"))
    {
        popup_add_student_to_base = new Popup_Add_Student_To_Base();
    } 
    ImGui::SameLine();
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
    ImGui::Checkbox("Режим редактирования", &edit_mode);
    ImGui::PopStyleColor();
    ImGui::Text("Список всех учеников");
    if (ImGui::BeginTable("students", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Фамилия и имя");
        ImGui::TableSetupColumn("No договора");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableSetupColumn("Возрастная группа");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        std::string name_input_buffer;
        int contract_input_buffer;
        int lesson_name_input_buffer;
        bool is_removed_input_buffer;
        for (int student_id = 0; student_id < all_students->size(); student_id++)
        {
            ImGui::PushID(student_id);
            name_input_buffer = all_students->at(student_id).get_name();
            contract_input_buffer = all_students->at(student_id).get_contract();
            is_removed_input_buffer = all_students->at(student_id).is_removed();
            ImGui::TableNextColumn(); 
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
            ImGui::SetNextItemWidth(-1);
            if (edit_mode)
            {
                if (ImGui::InputText("##ФИ", &name_input_buffer))
                {
                    all_students->at(student_id).set_name(name_input_buffer);
                };
            }
            else
            {
                ImGui::Text(name_input_buffer.c_str());
            }
            ImGui::TableNextColumn(); 
            if (!edit_mode) 
            {
                ImGui::Text(std::to_string(all_students->at(student_id).get_contract()).c_str());
            }
            else if (ImGui::InputInt("##Д-Р", &contract_input_buffer, ImGuiInputTextFlags_CharsDecimal))
            {
                if (contract_input_buffer < 0) contract_input_buffer = 0;
                all_students->at(student_id).set_contract(contract_input_buffer);
                
            }
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();
            for (int group_id = 0; group_id < all_groups->size(); group_id++) //TODO: literally doing twice as much work.
            //TODO: wrapping
            {
                if (all_groups->at(group_id).is_in_group(student_id)) 
                {
                    ImGui::BeginGroup();
                    ImGui::Text((std::to_string(all_groups->at(group_id).get_number()) + ", " + Day_Names[all_groups->at(group_id).get_day_of_the_week()]).c_str()); 
                    ImGui::EndGroup();
                }
            }

            ImGui::TableNextColumn(); 
            if (edit_mode) 
            {
                lesson_name_input_buffer = all_students->at(student_id).get_age_group() + 1; // plus one for a "not assigned" placeholder
                if (ImGui::Combo("##возрастная группа", &lesson_name_input_buffer, " не задана\0 4 года, дошкольная группа\0 5 лет, дошкольная группа\0 6 лет, дошкольная группа\0 7 лет, школьная группа\0 8 лет, школьная группа\0 9 лет, школьная группа\0 10-11 лет, школьная группа\0 12-13 лет, школьная группа\0\0"))
                {
                    all_students->at(student_id).set_age_group(lesson_name_input_buffer - 1);
                }
            }
            else
            {
                ImGui::Text(all_students->at(student_id).get_age_group_string().c_str());
            }

            ImGui::TableNextColumn(); 
            if (ImGui::Button("Посещение"))
            {
                *popup_edit_ignore_lessons_is_open = student_id;
            } 
            ImGui::SameLine();
            HelpMarker("student ignore help placeholder");
            ImGui::SameLine(); 
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
            if (ImGui::Checkbox("Выбыл?", &is_removed_input_buffer))
            {
                if (is_removed_input_buffer) all_students->at(student_id).remove();
                if (!is_removed_input_buffer) all_students->at(student_id).restore();
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}

bool groups_list(std::vector<Student>* all_students, std::vector<Group>* all_groups, std::vector<std::vector<Lesson_Info>>* all_lessons, std::vector<Calendar_Day>* all_days)
{
    ImGui::Begin("Список всех групп", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    static bool edit_mode = false;
    if (ImGui::Button("Вернуться к журналу")) return true;
    ImGui::SameLine();
    ImGui::Checkbox("Режим редактирования", &edit_mode);
    ImGui::Text("Список всех групп");
    for (int i = 0; i < all_groups->size(); i++)
    {
        
    }
    ImGui::End();
    return false;
}