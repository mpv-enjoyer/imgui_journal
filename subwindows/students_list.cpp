#include "students_list.h"
#include "../render/render.h"

Subwindow_Students_List::Subwindow_Students_List(Graphical* _graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
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
            if (journal->lesson_info(wday, merged_lesson_id)->get_group().is_deleted(student)) continue;
            lessons_per_student[student_id].push_back({wday, merged_lesson_id, internal_student_id});
        }
    }
}

bool Subwindow_Students_List::show_frame()
{
    ImGui::Begin("Список всех учеников", nullptr, WINDOW_FLAGS);

    if (ImGui::Button("Добавить ученика##в общий список"))
    {
        graphical->popup_add_student_to_base = new Popup_Add_Student_To_Base(graphical);
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
    if (ImGui::BeginTable("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Фамилия и имя", ImGuiTableColumnFlags_WidthFixed, 300.0F);
        ImGui::TableSetupColumn("No договора");
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
                    journal->set_student_name(student_id, name_input_buffer);
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
                ImGui::BeginGroup();
                
                std::string label = generate_label("##attend", {student_id, i});
                Attend_Data cached_data = current_group.get_attend_data(internal_student_id);
                std::string first_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(0).lesson_name_id);
                std::string second_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(1).lesson_name_id);
                if (j_attend_data(label.c_str(), &cached_data, first_name, second_name))
                {
                    journal->set_student_attend_data(current_wday, current_merged_lesson_id, internal_student_id, cached_data);
                }
                ImGui::AlignTextToFramePadding();
                std::string text = journal->Wday_name_short(current_info.wday) + ", " + current_group.get_description();
                ImGui::Text(text.c_str());

                //TODO CRITICAL: deletion here?
                ImGui::EndGroup();
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Выбыл?", &is_removed_input_buffer))
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
                if (is_removed_input_buffer) journal->remove_student(student_id);
                if (!is_removed_input_buffer) journal->restore_student(student_id);
                ImGui::PopStyleColor();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return false;
}