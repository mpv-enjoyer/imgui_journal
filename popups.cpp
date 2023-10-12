#include "main.h"

bool Popup_Add_Student_To_Group::show_frame()
{
    POPUP_INIT_FRAME("Добавление ученика в группу")
    {
        if (possible_student_ids.size() == 0) POPUP_CANCEL;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        text_filter.Draw("Поиск по ученикам");
        ImGui::PopStyleColor(1);
        bool select_visible = false;
        ImGui::BeginChild("Child window", ImVec2(0,400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (int i = 0; i < possible_student_descriptions.size(); i++)
        {
            if (!text_filter.PassFilter(possible_student_descriptions[i].c_str())) continue;
            if (current_selected_student == possible_student_ids[i])
            {
                std::string next_button_name = generate_label("Выбран.##", { i });
                j_button_selectable(next_button_name.c_str(), true);
                select_visible = true;
            }
            else
            {
                std::string next_button_name = generate_label("Выбрать##", { i });
                if (ImGui::Button(next_button_name.c_str())) current_selected_student = possible_student_ids[i];
            }
            ImGui::SameLine();
            ImGui::Text(possible_student_descriptions[i].c_str());
        }
        ImGui::EndChild();
        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("OK") && is_ok_possible(select_visible)) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Student_To_Group::accept_changes(const std::vector<std::vector<Lesson_Info>>& all_lessons,
    std::vector<Calendar_Day>& all_days, int current_mday, std::vector<int> visible_table_columns, int current_day_of_the_week)
{
    int current_group = get_current_group_id();
    int current_merged_lesson = get_merged_lesson();
    int new_student_id = (*all_groups)[current_group].add_student(get_added_student());
    for (int current_day_cell = 0; current_day_cell < visible_table_columns.size(); current_day_cell++)
    {
        all_days[visible_table_columns[current_day_cell]].add_student_to_group(current_group, get_added_student(), new_student_id);
        for (int internal_lesson_id = 0; internal_lesson_id < all_lessons[current_day_of_the_week][current_merged_lesson].get_lessons_size(); internal_lesson_id++)
        {
            Lesson current_lesson = {current_merged_lesson, internal_lesson_id};
            int status = STATUS_NO_DATA;
            if (visible_table_columns[current_day_cell] < current_mday - MDAY_DIFF) status = STATUS_NOT_AWAITED;
            all_days[visible_table_columns[current_day_cell]].set_status(current_lesson, get_added_student(), status);
        }
    }
}

bool Popup_Select_Day_Of_The_Week::show_frame()
{
    POPUP_INIT_FRAME("Выбрать день недели")
    {
        for (int i = 1; i < 7; i++)
        {
            std::string button_name = generate_label(Day_Names[i] + "##change_day_button", {});
            if (j_button_selectable(button_name.c_str(), day_of_the_week == i)) day_of_the_week = i;
            ImGui::SameLine();
        }
        //now for russian-styled calendar
        std::string button_name = generate_label(Day_Names[0] + "##change_day_button", {});
        if (j_button_selectable(button_name.c_str(), day_of_the_week == 0)) day_of_the_week = 0;
        ImGui::Combo("##Month", &month, Month_Names_Combo);
        ImGui::SameLine();
        ImGui::InputInt("##Year", &year, 0, 0);
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Select_Day_Of_The_Week::accept_changes(int& current_day_of_the_week, int& current_month, int& current_year)
{
    current_day_of_the_week = get_day_of_the_week();
    current_month = get_month();
    current_year = get_year();
    //current month days num is unsynced
    //...
}

bool Popup_Add_Student_To_Base::show_frame()
{
    POPUP_INIT_FRAME("Добавить ученика в базу")
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        ImGui::InputText("ФИ ученика", &name);
        if (ImGui::InputInt("No договора", &contract, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
        {
            if (contract < 0) contract = 0;
        }
        ImGui::Checkbox("Указать возрастную группу", &is_date_visible);
        if (is_date_visible)
        {
            ImGui::Combo("##Возрастная группа", &age_group, Age_Group_Names_Combo);
        }
        ImGui::PopStyleColor(1);
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Student_To_Base::accept_changes(std::vector<Student>& all_students)
{
    IM_ASSERT(check_ok());
    Student output;
    output.set_name(name);
    output.set_contract(contract);
    if (is_date_visible) output.set_age_group(age_group);
    all_students.push_back(output);
}

bool Popup_Add_Merged_Lesson_To_Journal::show_frame()
{
    POPUP_INIT_FRAME(("Добавить группу на " + Day_Names[day_of_the_week]).c_str())
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        ImGui::Combo("Программа", &combo_lesson_name_id, "ИЗО\0Лепка\0ИЗО+Лепка\0Лепка+ИЗО\0Дизайн\0Черчение\0Спецкурс\0\0");
        for (int i = 0; i < 2; i++)
        {
            if (combo_lesson_name_id == 2 || combo_lesson_name_id == 3) 
            {
                std::string lesson_name;
                if (i == 0) lesson_name = Lesson_Names[combo_lesson_name_id - 2];
                else lesson_name = Lesson_Names[3 - combo_lesson_name_id]; //mirrored
                ImGui::Text("%i. %s", i, lesson_name.c_str());
            }
            else if (i == 1) break; 
            ImGui::PushID(i);
            j_input_time("##LessonBegin", lesson_pairs[i].time_begin);
            ImGui::SameLine();
            ImGui::Text(" _ ");
            ImGui::SameLine();
            j_input_time("##LessonEnd", lesson_pairs[i].time_end);
            ImGui::PopID();
        }
        ImGui::SeparatorText("Данные новой группы");
        if (ImGui::InputInt("Номер", &group_number))
        {
            if (group_number < 0) group_number = 0;
        };
        ImGui::InputText("Описание", &group_comment);
        ImGui::PopStyleColor();

        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK; 
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Merged_Lesson_To_Journal::accept_changes(std::vector<Group>* all_groups, std::vector<Student>* all_students,
    std::vector<std::vector<Lesson_Info>>* all_lessons, std::vector<Calendar_Day>* all_days, const std::vector<int>& visible_table_columns, int current_mday)
{
    IM_ASSERT(check_ok());
    Lesson_Info lesson_info = Lesson_Info(all_groups);
    Group new_group = Group(all_students);
    new_group.set_number(group_number);
    new_group.set_day_of_the_week(day_of_the_week);
    new_group.set_comment(group_comment);
    int current_pair_name_id = combo_lesson_name_id;
    if (combo_lesson_name_id >= 2) current_pair_name_id -= 2;
    lesson_pairs[0].lesson_name_id = current_pair_name_id;
    lesson_info.add_lesson_pair(lesson_pairs[0]);
    if (combo_lesson_name_id == 2)
    {
        lesson_pairs[1].lesson_name_id = 1;
        lesson_info.add_lesson_pair(lesson_pairs[1]);
    } 
    if (combo_lesson_name_id == 3)
    {
        lesson_pairs[1].lesson_name_id = 0;
        lesson_info.add_lesson_pair(lesson_pairs[1]);
    } 
    all_groups->push_back(new_group);
    lesson_info.set_group(all_groups->size()-1);
    (*all_lessons)[day_of_the_week].push_back(lesson_info);
    (*all_lessons)[day_of_the_week][all_lessons[day_of_the_week].size() - 1].set_group(all_groups->size() - 1);
    for (int i = 0; i < visible_table_columns.size(); i++)
    {
        bool await_no_one = false;
        if (current_mday >= visible_table_columns[i]) await_no_one = true;
        (*all_days)[visible_table_columns[i]].add_merged_lesson(day_of_the_week, lesson_info, await_no_one, all_lessons[day_of_the_week].size() - 1);
    }
}

bool popup_edit_ignore_lessons(std::vector<std::vector<Lesson_Info>>* lessons_in_a_week, std::vector<Student>* all_students, int current_student_id, bool* ignore)
{
    ImGui::OpenPopup("Редактировать список посещения ученика (placeholder)");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Редактировать список посещения ученика (placeholder)", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        bool temp_lesson_ignore;
        Lesson current_lesson;
        for (int day_of_the_week = 0; day_of_the_week < 7; day_of_the_week++)
        {
            int day_of_the_week_ru = ( day_of_the_week + 1 ) % 7;
            ImGui::SeparatorText(Day_Names[day_of_the_week_ru].c_str());
            for (int merged_lesson_id = 0; merged_lesson_id < lessons_in_a_week->at(day_of_the_week_ru).size(); merged_lesson_id++)
            {
                current_lesson.merged_lesson_id = merged_lesson_id;
                for (int internal_lesson_id = 0; internal_lesson_id < lessons_in_a_week->at(day_of_the_week_ru)[merged_lesson_id].get_lessons_size(); internal_lesson_id++)
                {
                    if (!lessons_in_a_week->at(day_of_the_week_ru)[merged_lesson_id].should_attend(current_student_id)) continue;
                    current_lesson.internal_lesson_id = internal_lesson_id;
                    temp_lesson_ignore = all_students->at(current_student_id).is_ignored(current_lesson, day_of_the_week_ru);
                    if (ImGui::Checkbox(lessons_in_a_week->at(day_of_the_week_ru)[merged_lesson_id].get_description().c_str(), &temp_lesson_ignore))
                    {
                        if (temp_lesson_ignore) all_students->at(current_student_id).add_lesson_ignore_id(current_lesson, day_of_the_week_ru);
                        if (!temp_lesson_ignore) all_students->at(current_student_id).delete_lesson_ignore(current_lesson, day_of_the_week_ru);
                    }
                }
            }
        }
        if (ImGui::Button("OK"))
        {
            *ignore = false;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        }
        ImGui::SameLine();
        ImGui::BeginDisabled();
        if (ImGui::Button("Отмена"))
        {
            *ignore = true;
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        }
        ImGui::EndDisabled();
        ImGui::EndPopup();
    }
    return false;
}

bool popup_add_working_out(std::vector<Student>* all_students, std::vector<Group>* all_groups, std::vector<Calendar_Day>* all_days, std::vector<std::vector<Lesson_Info>>* all_lessons,
int current_group_id, int* selected_to_add, int first_mwday, int number_of_days, Workout_Info* lesson_to_workout)
{
    ImGui::OpenPopup("Добавление отработки");
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
    if (ImGui::BeginPopupModal("Добавление отработки", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        popup_add_working_out_filter.Draw("Поиск по ученикам");
        ImGui::PopStyleColor(1);
        bool select_student_visible = false;
        ImGui::BeginChild("Child window", ImVec2(500,300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
        for (int i = 0; i < possible_student_descriptions.size(); i++)
        {
            if (!popup_add_working_out_filter.PassFilter(possible_student_descriptions[i].c_str())) continue;
            if (*selected_to_add == possible_student_ids[i])
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
                ImGui::Button(("Выбран.##"+std::to_string(i)).c_str());
                ImGui::PopStyleColor(3);
                select_student_visible = true;
            }
            else
            {
                if (ImGui::Button(("Выбрать##"+std::to_string(i)).c_str())) *selected_to_add = possible_student_ids[i];
            }

            ImGui::SameLine();
            ImGui::Text(possible_student_descriptions[i].c_str());
        }
        ImGui::EndChild();
        if (!select_student_visible)
        {
            ImGui::BeginDisabled();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        //calendar
        int first_mwday_ru = (( first_mwday - 1 ) + 7) % 7 ;
        if (ImGui::BeginTable("##Календарь", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedSame))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            for (int i = 0; i < 7; i++)
            {
                const char* table_upper_name = Day_Names_Abbreviated[ ( (i + 1) + 7 ) % 7].c_str();
                ImGui::Text(table_upper_name);
                ImGui::TableNextColumn();
            }
            ImGui::TableSetColumnIndex(first_mwday_ru);
            for (int i = 0; i < number_of_days; i++)
            {
                if (select_student_visible)
                {
                    bool should_attend = false;
                    for (int j = 0; j < all_lessons->at((first_mwday + i) % 7).size(); j++)
                    {
                        should_attend = should_attend || all_lessons->at((first_mwday + i) % 7)[j].should_attend(*selected_to_add);
                    }
                    if (should_attend) 
                    {
                        if (popup_add_working_out_select_day == i)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
                            if (ImGui::SmallButton(std::to_string(i + 1).c_str())) popup_add_working_out_select_day = -1;
                            ImGui::PopStyleColor(3);
                        }
                        else 
                        if (ImGui::SmallButton(std::to_string(i + 1).c_str()))
                        {
                            popup_add_working_out_select_day = i;
                        }
                    }
                    else 
                    {
                        ImGui::BeginDisabled();
                        ImGui::SmallButton(std::to_string(i + 1).c_str());
                        ImGui::EndDisabled();
                    }
                }
                else
                {
                    ImGui::SmallButton(std::to_string(i + 1).c_str());
                }
                ImGui::TableNextColumn();
            }
            ImGui::EndTable();
        }
        if (!select_student_visible)
        {
            ImGui::EndDisabled();
        }
        
        if (select_student_visible && popup_add_working_out_select_day != -1)
        {
            for (int current_merged_lesson = 0; current_merged_lesson < all_lessons->at((first_mwday + popup_add_working_out_select_day) % 7).size(); current_merged_lesson++)
            {
                for (int current_internal_lesson = 0; current_internal_lesson < all_lessons->at((first_mwday + popup_add_working_out_select_day) % 7)[current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                {
                    if (all_lessons->at((first_mwday + popup_add_working_out_select_day) % 7)[current_merged_lesson].should_attend(*selected_to_add))
                    {
                        bool checkbox_value = popup_add_working_out_select_lesson.internal_lesson_id == current_internal_lesson && popup_add_working_out_select_lesson.merged_lesson_id == current_merged_lesson;
                        if (ImGui::Checkbox(all_lessons->at((first_mwday + popup_add_working_out_select_day) % 7)[current_merged_lesson].get_description(current_internal_lesson).c_str(), &checkbox_value) && checkbox_value) 
                        {
                            popup_add_working_out_select_lesson.internal_lesson_id = current_internal_lesson;
                            popup_add_working_out_select_lesson.merged_lesson_id = current_merged_lesson;
                        }
                    }
                }
            }
        }


        ImGui::EndGroup();
        

        if (ImGui::Button("OK", ImVec2(0, 0)) && *selected_to_add!=-1 && select_student_visible && popup_add_working_out_select_day != -1)
        {
            lesson_to_workout->lesson_pair = all_lessons->at((first_mwday + popup_add_working_out_select_day) % 7)[popup_add_working_out_select_lesson.merged_lesson_id].get_lesson_pair(popup_add_working_out_select_lesson.internal_lesson_id);

            lesson_to_workout->date = { 0, 0, 0, // second, minute, hour
            popup_add_working_out_select_day + 1, 9, 2023 - 1900}; // 1-based day, 0-based month, year since 1900 

            //TODO: (IMPORTANT) MONTH IS HARD CODED.

            lesson_to_workout->student_id = *selected_to_add;

            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            popup_add_working_out_filter.Clear();
            return true;
        } 
        ImGui::SameLine();
        if (ImGui::Button("Отмена", ImVec2(0, 0)))
        {
            *selected_to_add=-1;
            popup_add_working_out_filter.Clear();
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        } 
        ImGui::EndPopup();
    }
    return false;
}
