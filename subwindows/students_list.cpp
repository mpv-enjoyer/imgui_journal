#include "students_list.h"
#include "../render/render.h"

Subwindow_Students_List::Subwindow_Students_List(Graphical* _graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    update_lessons_per_student();
}

void Subwindow_Students_List::update_lessons_per_student()
{
    lessons_per_student.clear();
    lessons_per_student = std::vector<std::vector<Lesson_Info_Position>>(journal->student_count());
    for (int student_id = 0; student_id < journal->student_count(); student_id++)
    {
        update_lessons_per_student(student_id);
        auto& student = PTRREF(journal->student(student_id));
    }
}

void Subwindow_Students_List::update_lessons_per_student(int student_id)
{
    lessons_per_student[student_id].clear();
    auto& student = PTRREF(journal->student(student_id));
    for (int wday = 0; wday < 7; wday++)
    {
        for (int merged_lesson_id = 0; merged_lesson_id < journal->lesson_info_count(wday); merged_lesson_id++)
        {
            int internal_student_id = journal->lesson_info(wday, merged_lesson_id)->get_group().find_student(student);
            if (internal_student_id == -1) continue;
            // Do not discard deleted students because we need to be able to restore them
            lessons_per_student[student_id].push_back({wday, merged_lesson_id, internal_student_id});
        }
    }
}

bool Subwindow_Students_List::show_frame()
{
    bool edit_mode = graphical->edit_mode;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, background);
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Список всех учеников", nullptr, WINDOW_FLAGS);
    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        ImGui::PopStyleColor(); //BG
        return true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Добавить ученика##в общий список"))
    {
        graphical->popup_add_student_to_base = new Popup_Add_Student_To_Base(graphical);
    } 
    ImGui::Text("Список всех учеников");

    if (lessons_per_student.size() != journal->student_count())
        update_lessons_per_student();

    ImGui::BeginChild("Child", ImVec2(0, TABLE_BOTTOM_OFFSET_PXLS), true, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTable("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX))
    {
        ImGui::TableSetupColumn("Фамилия и имя");
        ImGui::TableSetupColumn("Номер договора");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableSetupColumn("Действия");
        ImGui::TableHeadersRow();
        std::string name_input_buffer;
        int contract_input_buffer;
        int lesson_name_input_buffer;
        bool is_removed_input_buffer;
        for (int student_id = 0; student_id < journal->student_count(); student_id++)
        {
            const Student* current_student = journal->student(student_id);
            if (!edit_mode && current_student->is_removed()) continue;
            if (current_student->is_removed()) ImGui::BeginDisabled();
            ImGui::PushID(student_id);
            name_input_buffer = current_student->get_name();
            contract_input_buffer = current_student->get_contract();
            is_removed_input_buffer = current_student->is_removed();
            ImGui::TableNextColumn(); 
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText("##ФИ", &name_input_buffer))
            {
                journal->set_student_name(student_id, name_input_buffer);
            };
            ImGui::TableNextColumn(); 
            if (ImGui::InputInt("##Д-Р", &contract_input_buffer, ImGuiInputTextFlags_CharsDecimal))
            {
                if (contract_input_buffer < 0) contract_input_buffer = 0;
                journal->set_student_contract(student_id, contract_input_buffer);
            }
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();

            for (int i = 0; i < lessons_per_student[student_id].size(); i++)
            {
                const Lesson_Info_Position current_info = lessons_per_student[student_id][i];
                const int current_wday = lessons_per_student[student_id][i].wday;
                const int current_merged_lesson_id = lessons_per_student[student_id][i].merged_lesson;
                const auto& current_lesson_info = journal->lesson_info(current_wday, current_merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
                const int internal_student_id = lessons_per_student[student_id][i].internal_student_id;
                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::BeginDisabled();
                ImGui::BeginGroup();
                
                std::string label = generate_label("##attend", {student_id, i});
                Attend_Data cached_data = current_group.get_attend_data(internal_student_id);
                std::string first_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(0).lesson_name_id);
                if (current_lesson_info->get_lessons_size() == 2)
                {
                    std::string second_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(1).lesson_name_id);
                    if (j_attend_data(label.c_str(), &cached_data, first_name, second_name))
                    {
                        journal->set_student_attend_data(current_wday, current_merged_lesson_id, internal_student_id, cached_data);
                    }
                }
                else
                {
                    ImGui::Text(first_name.c_str());
                }
                ImGui::AlignTextToFramePadding();

                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::EndDisabled();
                ImGui::SameLine();
                if (!current_group.is_deleted(PTRREF(journal->student(student_id))))
                {
                    std::string label_delete = generate_label("Удалить из группы##", {student_id, i});
                    if (j_button_dangerous(label_delete.c_str()))
                    {
                        journal->remove_student_from_group(current_wday, current_merged_lesson_id, student_id);
                    }
                }
                else
                {
                    std::string label_restore = generate_label("Восстановить в группе##", {student_id, i});
                    if (j_button_colored(label_restore.c_str(), 0.1, 0.9, 0.1))
                    {
                        journal->restore_student_to_group(current_wday, current_merged_lesson_id, student_id);
                    }
                }

                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::BeginDisabled();
                std::string text = journal->Wday_name_short(current_info.wday) + ", " + current_group.get_description();
                ImGui::Text(text.c_str());

                //TODO CRITICAL: deletion here?
                ImGui::EndGroup();
                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::EndDisabled();
                if (i != lessons_per_student[student_id].size() - 1)
                    ImGui::Separator();
            }
            if (current_student->is_removed()) ImGui::EndDisabled();
            ImGui::TableNextColumn();
            if (!edit_mode)
            {
                if (j_button_dangerous("Удалить ученика"))
                {
                    graphical->popup_confirm_delete_student = new Popup_Confirm_Delete_Student(graphical, student_id);
                }
            }
            else
            {
                if (is_removed_input_buffer && j_button_colored("Восстановить ученика", 0.1, 0.9, 0.1))
                {
                    journal->restore_student(student_id);
                }
                else if (!is_removed_input_buffer && j_button_dangerous("Удалить ученика"))
                {
                    graphical->popup_confirm_delete_student = new Popup_Confirm_Delete_Student(graphical, student_id);
                }
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleColor(); //BG
    return false;
}