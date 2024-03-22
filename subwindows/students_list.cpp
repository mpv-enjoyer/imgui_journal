#include "students_list.h"
#include "../render/render.h"

void Subwindow_Students_List::update_lessons_per_student()
{
    lessons_per_student.clear();
    lessons_per_student = std::vector<std::vector<Lesson_Info_Position>>(Journal::student_count());
    for (int student_id = 0; student_id < Journal::student_count(); student_id++)
    {
        auto& student = PTRREF(Journal::student(student_id));
        for (int wday = 0; wday < 7; wday++)
        {
            for (int merged_lesson_id = 0; merged_lesson_id < Journal::lesson_info_count(wday); merged_lesson_id++)
            {
                if (Journal::lesson_info(wday, merged_lesson_id)->get_group().check_no_attend_data(student))
                {
                    lessons_per_student[student_id].push_back({wday, merged_lesson_id});
                }
            }
        }
    }
}

void Subwindow_Students_List::update_lessons_per_student(int student_id)
{
    lessons_per_student[student_id].clear();
    auto& student = PTRREF(Journal::student(student_id));
    for (int wday = 0; wday < 7; wday++)
    {
        for (int merged_lesson_id = 0; merged_lesson_id < Journal::lesson_info_count(wday); merged_lesson_id++)
        {
            if (Journal::lesson_info(wday, merged_lesson_id)->get_group().check_no_attend_data(student))
            {
                lessons_per_student[student_id].push_back({wday, merged_lesson_id});
            }
        }
    }
}

bool Subwindow_Students_List::show_frame()
{
    ImGui::Begin("Список всех учеников", nullptr, WINDOW_FLAGS);

    if (ImGui::Button("Добавить ученика##в общий список"))
    {
        Graphical::popup_add_student_to_base = new Popup_Add_Student_To_Base();
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
    if (ImGui::BeginTable("students", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Фамилия и имя", ImGuiTableColumnFlags_WidthFixed, 300.0F);
        ImGui::TableSetupColumn("No договора");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableSetupColumn("Возрастная группа");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        std::string name_input_buffer;
        int contract_input_buffer;
        int lesson_name_input_buffer;
        bool is_removed_input_buffer;
        for (int student_id = 0; student_id < Journal::student_count(); student_id++)
        {
            const Student* current_student = Journal::student(student_id);
            if (!edit_mode && current_student->is_removed()) continue;
            ImGui::PushID(student_id);
            name_input_buffer = current_student->get_name();
            contract_input_buffer = current_student->get_contract();
            is_removed_input_buffer = current_student->is_removed();
            ImGui::TableNextColumn(); 
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
            ImGui::SetNextItemWidth(-1);
            if (edit_mode)
            {
                if (ImGui::InputText("##ФИ", &name_input_buffer))
                {
                    Journal::set_student_name(student_id, name_input_buffer);
                };
            }
            else
            {
                ImGui::AlignTextToFramePadding();
                ImGui::Text(name_input_buffer.c_str());
            }
            ImGui::TableNextColumn(); 
            if (!edit_mode) 
            {
                ImGui::AlignTextToFramePadding();
                ImGui::Text(std::to_string(current_student->get_contract()).c_str());
            }
            else if (ImGui::InputInt("##Д-Р", &contract_input_buffer, ImGuiInputTextFlags_CharsDecimal))
            {
                if (contract_input_buffer < 0) contract_input_buffer = 0;
                Journal::set_student_contract(student_id, contract_input_buffer);
            }
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();

            for (int i = 0; i < lessons_per_student[student_id].size(); i++)
            {
                const int current_wday = lessons_per_student[student_id][i].wday;
                const int current_merged_lesson_id = lessons_per_student[student_id][i].merged_lesson;
                const auto& current_lesson_info = Journal::lesson_info(current_wday, current_merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
            }

            for (int group_id = 0; group_id < Journal::lesson_info_count(); group_id++) //TODO: literally doing twice as much work.
            //TODO: wrapping
            {
                if (all_groups.at(group_id)->is_in_group(current_student)) 
                {
                    bool value = all_groups.at(group_id)->is_deleted(current_student);
                    if (!edit_mode && value) continue;
                    ImGui::BeginGroup();
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text((std::to_string(all_groups[group_id]->get_number()) + ", " + Day_Names[all_groups[group_id]->get_day_of_the_week()]).c_str()); 
                    ImGui::SameLine();
                    if (edit_mode)
                    {
                        auto label = generate_label("Удалить из группы?##checkbox", { group_id, student_id });
                        if (ImGui::Checkbox(label.c_str(), &value))
                        {
                            if (value) all_groups[group_id]->delete_student(current_student);
                            if (!value) all_groups[group_id]->restore_student(current_student);
                        }
                    }
                    else
                    {
                        if (!value)
                        {
                            auto label = generate_label("Удалить из группы##checkbox", { group_id, student_id });
                            if (j_button_dangerous(label.c_str())) all_groups[group_id]->delete_student(current_student);
                        }
                    }
                    ImGui::EndGroup();
                }
            }
            ImGui::TableNextColumn(); 
            if (edit_mode) 
            {
                lesson_name_input_buffer = current_student.get_age_group() + 1; // plus one for a "not assigned" placeholder
                if (ImGui::Combo("##возр", &lesson_name_input_buffer, " не задана\0 4 года, дошкольная группа\0 5 лет, дошкольная группа\0 6 лет, дошкольная группа\0 7 лет, школьная группа\0 8 лет, школьная группа\0 9 лет, школьная группа\0 10-11 лет, школьная группа\0 12-13 лет, школьная группа\0\0"))
                {
                    current_student.set_age_group(lesson_name_input_buffer - 1);
                }
            }
            else
            {
                ImGui::Text(current_student.get_age_group_string().c_str());
            }

            ImGui::TableNextColumn();
            if (edit_mode && ImGui::Checkbox("Выбыл?", &is_removed_input_buffer))
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
                if (is_removed_input_buffer) current_student.remove();
                if (!is_removed_input_buffer) current_student.restore();
                ImGui::PopStyleColor();
            }
            if (!edit_mode && !is_removed_input_buffer && j_button_dangerous("Выбыл"))
            {
                current_student.remove();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}