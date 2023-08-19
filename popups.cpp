#include "main.h"

bool validate_time_int(std::string input_string, int awaited_symbols, int upper_limit)
{
    if (input_string.size() > awaited_symbols) return false;
    for (int i = 0; i < input_string.size(); i++)
    {
        if (input_string[i] < '0' || input_string[i] > '9') return false;
    }
    if (std::stoi(input_string) >= upper_limit) return false;
    return true;
}

bool popup_add_student_to_group(std::vector<Student>* all_students, std::vector<Group>* all_groups, std::vector<Calendar_Day>* all_days, int current_group_id, int* selected_to_add)
{
    ImGui::OpenPopup("Добавление ученика в группу");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
    for (int i = 0; i < all_students->size(); i++)
    {
        if (all_students->at(i).is_removed()) continue;
        if (all_groups->at(current_group_id).is_in_group(i)) continue;
        possible_student_descriptions.push_back((all_students->at(i).get_name() + " (" + std::to_string(all_students->at(i).get_contract()) + ")"));
        possible_student_ids.push_back(i);
    }
    if (possible_student_ids.size() == 0) 
    {
        *selected_to_add = -1;
        return true;
    }
    if (ImGui::BeginPopupModal("Добавление ученика в группу", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        popup_add_student_to_group_filter.Draw("Поиск по ученикам");
        ImGui::PopStyleColor(1);
        bool select_visible = false;
        ImGui::BeginChild("Child window", ImVec2(0,400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (int i = 0; i < possible_student_descriptions.size(); i++)
        {
            if (!popup_add_student_to_group_filter.PassFilter(possible_student_descriptions[i].c_str())) continue;
            if (*selected_to_add == possible_student_ids[i])
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
                ImGui::Button(("Выбран.##"+std::to_string(i)).c_str());
                ImGui::PopStyleColor(3);
                select_visible = true;
            }
            else
            {
                if (ImGui::Button(("Выбрать##"+std::to_string(i)).c_str())) *selected_to_add = possible_student_ids[i];
            }

            ImGui::SameLine();
            ImGui::Text(possible_student_descriptions[i].c_str());
        }
        ImGui::EndChild();
        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("OK", ImVec2(0, 0)) && *selected_to_add!=-1 && select_visible)
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            popup_add_student_to_group_filter.Clear();
            return true;
        } 
        ImGui::SameLine();
        if (ImGui::Button("Отмена", ImVec2(0, 0)))
        {
            *selected_to_add=-1;
            popup_add_student_to_group_filter.Clear();
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::EndPopup();
    }
    return false;
}

bool popup_select_day_of_the_week(int* selected_day_of_the_week, int* selected_month)
{
    ImGui::OpenPopup("Выбрать день недели");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Выбрать день недели", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {


        for (int i = 1; i < 7; i++)
        {
            bool is_selected = false;
            if (*selected_day_of_the_week == i)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
                is_selected = true;
            }

            if (ImGui::Button(Day_Names[i].c_str()))
            {
                *selected_day_of_the_week = i;
            }

            if (is_selected) ImGui::PopStyleColor(3);                

            ImGui::SameLine();
        }
        bool is_selected = false;
        if (*selected_day_of_the_week == 0)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
            is_selected = true;
        }

        if (ImGui::Button(Day_Names[0].c_str()))
        {
            *selected_day_of_the_week = 0;
        }
        
        if (is_selected) ImGui::PopStyleColor(3);

        ImGui::Combo("##Month", selected_month, "Январь\0Февраль\0Март\0Апрель\0Май\0Июнь\0Июль\0Август\0Сентябрь\0Октябрь\0Ноябрь\0Декабрь\0\0");

        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::SameLine();
        if (ImGui::Button("Отмена"))
        {
            *selected_month = -1;
            *selected_day_of_the_week = -1;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::EndPopup();
    }
    return false;
}

bool popup_add_student_to_base(Student* new_student, bool* ignore, bool erase_input)
{
    ImGui::OpenPopup("Добавить ученика в базу");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Добавить ученика в базу", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static bool is_date_visible = false;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        static std::string new_student_name;
        if (erase_input) new_student_name = "";
        ImGui::InputText("ФИ ученика", &new_student_name);
        static int new_student_contract = 0; 
        if(erase_input) new_student_contract = 0;
        if (ImGui::InputInt("No договора", &new_student_contract, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
        {
            if (new_student_contract < 0) new_student_contract = 0;
        }
        static int age_group = 0;
        if (erase_input) 
        {
            age_group = 0;
        }
        ImGui::Checkbox("Указать возрастную группу", &is_date_visible);
        if (is_date_visible)
        {
            ImGui::Combo("##Возрастная группа", &age_group, " 4 года, дошкольная группа\0 5 лет, дошкольная группа\0 6 лет, дошкольная группа\0 7 лет, школьная группа\0 8 лет, школьная группа\0 9 лет, школьная группа\0 10-11 лет, школьная группа\0 12-13 лет, школьная группа\0\0");
        }
        ImGui::PopStyleColor(1);
        if (ImGui::Button("OK"))
        {
            if (new_student_contract < 0) return false;
            new_student->set_contract(new_student_contract);
            new_student->set_name(new_student_name);
            if (is_date_visible)
            {
                new_student->set_age_group(age_group);
            }
            *ignore = false;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::SameLine();
        if (ImGui::Button("Отмена"))
        {
            *ignore = true;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        }
        ImGui::EndPopup();
    }
    return false;
}

bool popup_add_merged_lesson_to_journal(std::vector<Group>* all_groups, Lesson_Info* new_lesson_info, int current_day_of_the_week, bool* ignore, bool erase_input)
{
    ImGui::OpenPopup(("Добавить урок на " + Day_Names[current_day_of_the_week]).c_str());
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    static Lesson_Pair new_lesson_pairs[] = {{0,0,0}, {0,0,0}};
    std::string time_buffer;
    if (ImGui::BeginPopupModal(("Добавить урок на " + Day_Names[current_day_of_the_week]).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static int new_lesson_group_id = -1;
        static int new_combo_lesson_name_id = 0;
        if (erase_input)
        {
            new_lesson_group_id = -1;
            new_combo_lesson_name_id = 0;
            new_lesson_pairs[0] = {0,0,0};
            new_lesson_pairs[1] = {0,0,0};
        }
        std::string preview_value = "";
        if (new_lesson_group_id != -1) preview_value = all_groups->at(new_lesson_group_id).get_description();
        if (ImGui::BeginCombo("Группа", preview_value.c_str()))
        {
            for (int n = 0; n < all_groups->size(); n++)
            {
                const bool is_selected = (new_lesson_group_id == n);
                if (ImGui::Selectable(all_groups->at(n).get_description().c_str(), is_selected))
                    new_lesson_group_id = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Combo("Программа", &new_combo_lesson_name_id, "ИЗО\0Лепка\0ИЗО+Лепка\0Лепка+ИЗО\0Дизайн\0Черчение\0Спецкурс\0\0");
        //ImGui::Combo("День недели", &new_lesson_day_of_the_week, "Понедельник\0Вторник\0Среда\0Четверг\0Пятница\0Суббота\0Воскресенье");
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        for (int i = 0; i < 2; i++)
        {
            if (new_combo_lesson_name_id == 2 || new_combo_lesson_name_id == 3) 
            {
                ImGui::Text("%i. ", i);
            }
            else if (i==1) break; 
            ImGui::PushID(i);
            time_buffer = std::to_string(new_lesson_pairs[i].time_begin.hours); if (time_buffer.size()==1) time_buffer = "0" + time_buffer; 
            if (ImGui::InputText("##HoursBegin", &time_buffer, ImGuiInputTextFlags_AlwaysOverwrite) && validate_time_int(time_buffer, 2, 24)) new_lesson_pairs[i].time_begin.hours = std::stoi(time_buffer);
            ImGui::SameLine();
            ImGui::Text(":");
            ImGui::SameLine();
            time_buffer = std::to_string(new_lesson_pairs[i].time_begin.minutes); if (time_buffer.size()==1) time_buffer = "0" + time_buffer; 
            if (ImGui::InputText("##MinutesBegin", &time_buffer, ImGuiInputTextFlags_AlwaysOverwrite) && validate_time_int(time_buffer, 2, 60)) new_lesson_pairs[i].time_begin.minutes = std::stoi(time_buffer);
            ImGui::SameLine();
            ImGui::Text(" - ");
            ImGui::SameLine();
            time_buffer = std::to_string(new_lesson_pairs[i].time_end.hours); if (time_buffer.size()==1) time_buffer = "0" + time_buffer; 
            if (ImGui::InputText("##HoursEnd", &time_buffer, ImGuiInputTextFlags_AlwaysOverwrite) && validate_time_int(time_buffer, 2, 24)) new_lesson_pairs[i].time_end.hours = std::stoi(time_buffer);
            ImGui::SameLine();
            ImGui::Text(":");
            ImGui::SameLine();
            time_buffer = std::to_string(new_lesson_pairs[i].time_end.minutes); if (time_buffer.size()==1) time_buffer = "0" + time_buffer; 
            if (ImGui::InputText("##MinutesEnd", &time_buffer, ImGuiInputTextFlags_AlwaysOverwrite) && validate_time_int(time_buffer, 2, 60)) new_lesson_pairs[i].time_end.minutes = std::stoi(time_buffer);
            ImGui::PopID();
        }
        ImGui::PopStyleColor();

        if (ImGui::Button("OK"))
        {
            *ignore = false;
            int current_pair_name_id = new_combo_lesson_name_id;
            if (new_combo_lesson_name_id >= 2) current_pair_name_id -= 2;
            new_lesson_pairs[0].lesson_name_id = current_pair_name_id;
            new_lesson_info->add_lesson_pair(new_lesson_pairs[0]);
            if (new_combo_lesson_name_id == 2)
            {
                new_lesson_pairs[1].lesson_name_id = 1;
                new_lesson_info->add_lesson_pair(new_lesson_pairs[1]);
            } 
            if (new_combo_lesson_name_id == 3)
            {
                new_lesson_pairs[1].lesson_name_id = 0;
                new_lesson_info->add_lesson_pair(new_lesson_pairs[1]);
            } 
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::SameLine();
        if (ImGui::Button("Отмена"))
        {
            *ignore = true;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        }
        ImGui::EndPopup();
    }
    return false;
}