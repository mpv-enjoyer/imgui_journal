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
    auto indexes = sort_indexes_by_ptr(journal->students());
    lessons_per_student.clear();
    lessons_per_student = std::vector<std::pair<std::vector<Lesson_Info_Position>, int>>(journal->student_count());
    IM_ASSERT(indexes.size() == lessons_per_student.size());
    for (int index = 0; index < journal->student_count(); index++)
    {
        update_lessons_per_student(indexes[index], index);
    }
}

void Subwindow_Students_List::update_lessons_per_student(int student_id, int index)
{
    lessons_per_student[index].first.clear();
    lessons_per_student[index].second = student_id;
    auto& student = PTRREF(journal->student(student_id));
    for (int wday = 0; wday < 7; wday++)
    {
        for (int merged_lesson_id = 0; merged_lesson_id < journal->lesson_info_count(wday); merged_lesson_id++)
        {
            int internal_student_id = journal->lesson_info(wday, merged_lesson_id)->get_group().find_student(student);
            if (internal_student_id == -1) continue;
            // Do not discard deleted students because we need to be able to restore them
            lessons_per_student[index].first.push_back({wday, merged_lesson_id, internal_student_id});
        }
    }
}

void Subwindow_Students_List::append_students_to_begin()
{
    if (lessons_per_student.size() == journal->student_count()) return;
    const int index = 0;
    for (int i = lessons_per_student.size(); i < journal->student_count(); i++)
    {
        lessons_per_student.insert(lessons_per_student.begin(),std::pair<std::vector<Lesson_Info_Position>, int>());
        update_lessons_per_student(i, 0);
    }
}

bool Subwindow_Students_List::show_frame()
{
    bool edit_mode = graphical->edit_mode;
    bool is_current = journal->get_state() == Journal::State::Fullaccess;
    
    if (std::string(text_filter.InputBuf).size() > 0) ImGui::PushStyleColor(ImGuiCol_ChildBg, background_filtered);
    else ImGui::PushStyleColor(ImGuiCol_ChildBg, background);
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
    
    if (is_current)
    {
        ImGui::SameLine();
        if (ImGui::Button("Добавить ученика##в общий список"))
        {
            graphical->popup_add_student_to_base = new Popup_Add_Student_To_Base(graphical);
        } 
    }

    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
    ImGui::SameLine();
    text_filter.Draw("Поиск", 140);
    if (std::string(text_filter.InputBuf).size() > 0)
    {
        ImGui::SameLine();
        if (j_button_colored("Очистить поиск", 0.7, 0.3, 0.3))
        {
            text_filter.Clear();
        }
    }
    ImGui::PopStyleColor();

    ImGui::Text("Список всех учеников");

    if (!is_current)
    {
        ImVec4 red = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
        ImGui::SameLine();
        ImGui::TextColored(red, "Вы смотрите данные другого месяца (%s). Редактирование доступно только для текущего месяца.", Month_Names[journal->current_month()].c_str());
    }

    if (should_update_students)
    {
        update_lessons_per_student();
        should_update_students = false;
    }

    if (lessons_per_student.size() != journal->student_count())
        append_students_to_begin();

    static bool sizing = false;
    ImGui::Checkbox("DBG SIZING", &sizing);
    #error Not ready for release

    ImGui::BeginChild("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTable("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Номер д-ра", sizing ? ImGuiTableColumnFlags_WidthFixed : 0);
        ImGui::TableSetupColumn("Фамилия и имя");
        ImGui::TableSetupColumn("Группы");
        ImGui::TableSetupColumn("Действия", sizing ? ImGuiTableColumnFlags_WidthFixed : 0);
        ImGui::TableHeadersRow();
        std::string name_input_buffer;
        int contract_input_buffer;
        int lesson_name_input_buffer;
        bool is_removed_input_buffer;

        for (int index = 0; index < journal->student_count(); index++)
        {
            const int student_id = lessons_per_student[index].second;
            const Student* current_student = journal->student(student_id);
            const std::string search_description = std::to_string(current_student->get_contract()) + " " + current_student->get_name();
            if (!text_filter.PassFilter(search_description.c_str())) continue;
            if (!edit_mode && current_student->is_removed()) continue;
            if (current_student->is_removed()) ImGui::BeginDisabled();
            if (!is_current) ImGui::BeginDisabled();
            ImGui::PushID(student_id);
            name_input_buffer = current_student->get_name();
            contract_input_buffer = current_student->get_contract();
            is_removed_input_buffer = current_student->is_removed();
            ImGui::TableNextColumn(); 
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.6f));
            if (ImGui::InputInt("##д-р", &contract_input_buffer, ImGuiInputTextFlags_CharsDecimal))
            {
                if (contract_input_buffer < 0) contract_input_buffer = 0;
                journal->set_student_contract(student_id, contract_input_buffer);
            }
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText("##фи", &name_input_buffer))
            {
                journal->set_student_name(student_id, name_input_buffer);
            };
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();

            for (int i = 0; i < lessons_per_student[index].first.size(); i++)
            {
                const Lesson_Info_Position current_info = lessons_per_student[index].first[i];
                const int current_wday = current_info.wday;
                const int current_merged_lesson_id = current_info.merged_lesson;
                const auto& current_lesson_info = journal->lesson_info(current_wday, current_merged_lesson_id);
                const auto& current_group = current_lesson_info->get_group();
                const int internal_student_id = current_info.internal_student_id;
                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::BeginDisabled();
                ImGui::BeginGroup();

                std::string text = journal->Wday_name_short(current_info.wday) + ", " + current_group.get_description();
                ImGui::Text(text.c_str());

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

                ImGui::SameLine();
                std::string button_label = generate_label("Переместить##move", {student_id, i});
                if (ImGui::Button(button_label.c_str()))
                {
                    graphical->popup_move_student_to_group = new Popup_Move_Student_To_Group(graphical, current_lesson_info, current_wday, current_merged_lesson_id, student_id, &should_update_students);
                }
                
                ImGui::EndGroup();
                if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::EndDisabled();
                if (i != lessons_per_student[index].first.size() - 1)
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
            if (!is_current) ImGui::EndDisabled();
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleColor(); //BG
    return false;
}