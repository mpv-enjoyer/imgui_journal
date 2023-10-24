#include "popups.h"
#include "helper_functions.h"
#include "student.h"
#include "group.h"
#include "lesson_info.h"
#include "calendar_day.h"

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
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Student_To_Group::accept_changes(const std::vector<Visible_Day>& visible_days)
//this function might lead to SEGFAULT if trying to add student to a group which has multiple lessons in a week.
//shouldn't be a problem for me though because my project assumes that every group has exactly one lesson
//but in case someone else wants to use it, please rewrite this
{
    Group& current_group = get_current_group();
    int current_merged_lesson_id = get_merged_lesson_known_id();
    int new_student_id = current_group.add_student(get_added_student());
    for (int current_day_cell = 0; current_day_cell < visible_days.size(); current_day_cell++)
    {
        visible_days[current_day_cell].day.add_student_to_group(current_group, get_added_student(), new_student_id);
        for (int internal_lesson_id = 0; internal_lesson_id < current_lesson.get_lessons_size(); internal_lesson_id++)
        {
            Lesson current_known_lesson = {current_merged_lesson_id, internal_lesson_id};
            int status = STATUS_NO_DATA;
            if (!(visible_days[current_day_cell].is_future || visible_days[current_day_cell].is_today)) status = STATUS_NOT_AWAITED;
            visible_days[current_day_cell].day.set_status(current_known_lesson, new_student_id, status);
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
        ImGui::SameLine(); print_error();
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
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Student_To_Base::accept_changes(std::vector<Student&>& all_students)
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
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Merged_Lesson_To_Journal::accept_changes(std::vector<Group&>& all_groups, std::vector<Lesson_Info&>& lessons_in_this_day, const std::vector<Visible_Day>& visible_days)
{
    IM_ASSERT(check_ok());
    Group new_group = Group();
    Lesson_Info lesson_info = Lesson_Info(new_group);
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
    all_groups.push_back(new_group);
    lesson_info.set_group(new_group);
    int new_merged_lesson_known_id = lessons_in_this_day.size();
    for (int i = 0; i < lessons_in_this_day.size(); i++)
    {
        if (lessons_in_this_day[i] < lesson_info)
        {
            new_merged_lesson_known_id = i;
            break;
        }
    }
    lessons_in_this_day.insert(lessons_in_this_day.begin() + new_merged_lesson_known_id, lesson_info);
    //(*all_lessons)[day_of_the_week][(*all_lessons)[day_of_the_week].size() - 1].set_group(all_groups[all_groups.size() - 1]); useless?
    for (int i = 0; i < visible_days.size(); i++)
    {
        bool await_no_one = false;
        if (!(visible_days[i].is_future || visible_days[i].is_today)) await_no_one = true;
        visible_days[i].day.add_merged_lesson(lesson_info, await_no_one, new_merged_lesson_known_id);
    }
}

/*bool popup_edit_ignore_lessons(std::vector<std::vector<Lesson_Info>>* lessons_in_a_week, std::vector<Student>* all_students, int current_student_id, bool* ignore)
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
}*/

bool Popup_Add_Working_Out::show_frame()
{
    if (possible_student_ids.size() == 0)
    {
        cancel();
        return true;
    }
    POPUP_INIT_FRAME("Добавление отработки")
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
        filter.Draw("Поиск по ученикам");
        ImGui::PopStyleColor(1);
        bool select_student_visible = false;
        ImGui::BeginChild("Child window", ImVec2(500,300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
        for (int i = 0; i < possible_student_descriptions.size(); i++)
        {
            if (!filter.PassFilter(possible_student_descriptions[i].c_str())) continue;
            if (select_student == possible_student_ids[i])
            {
                std::string student_button_name = generate_label("Выбран.##", { i });
                j_button_selectable(student_button_name.c_str(), true);
                select_student_visible = true;
            }
            else
            {
                std::string student_button_name = generate_label("Выбрать##", { i });
                if (ImGui::Button(student_button_name.c_str())) 
                {
                    select_student = possible_student_ids[i];
                    select_student_visible = true;
                }
            }
            ImGui::SameLine();
            ImGui::Text(possible_student_descriptions[i].c_str());
        }
        ImGui::EndChild();
        if (!select_student_visible)
        {
            select_student = -1;
            ImGui::BeginDisabled();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        //calendar
        int first_mwday_ru = (( first_mwday - 1 ) + 7) % 7 ;
        bool is_calendar_filled = false;
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
            for (int i = 0; i < count_mday; i++)
            {
                if (select_student_visible)
                {
                    bool should_attend = false;
                    for (int j = 0; j < all_lessons[(first_mwday + i) % 7].size(); j++)
                    {
                        should_attend = should_attend || all_lessons[(first_mwday + i) % 7][j].should_attend(all_students[select_student]);
                    }
                    if (should_attend) 
                    {
                        is_calendar_filled = true;
                        if (select_day == i)
                        {
                            if (j_button_selectable(std::to_string(i + 1).c_str(), true, true)) select_day = -1;
                        }
                        else 
                        if (ImGui::SmallButton(c_str_int(i + 1)))
                        {
                            select_day = i;
                        }
                    }
                    else 
                    {
                        ImGui::BeginDisabled();
                        ImGui::SmallButton(c_str_int(i + 1));
                        ImGui::EndDisabled();
                    }
                }
                else
                {
                    ImGui::SmallButton(c_str_int(i + 1));
                }
                ImGui::TableNextColumn();
            }
            if (!is_calendar_filled && select_student_visible)
            {
                select_day = -1;
            }
            ImGui::EndTable();
        }
        if (!select_student_visible)
        {
            ImGui::EndDisabled();
        }
        if (select_student_visible && select_day != -1)
        {
            bool is_visible = false;
            Lesson current_lesson;
            for (int current_merged_lesson = 0; current_merged_lesson < all_lessons.at((first_mwday + select_day) % 7).size(); current_merged_lesson++)
            {
                current_lesson.merged_lesson_id = current_merged_lesson;
                for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[(first_mwday + select_day) % 7][current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                {
                    current_lesson.internal_lesson_id = current_internal_lesson;
                    if (select_month == caller_month && select_year == caller_year && select_day == caller_mday && current_lesson == caller_lesson) continue;
                    if (all_lessons.at((first_mwday + select_day) % 7)[current_merged_lesson].should_attend(all_students[select_student]))
                    {
                        bool checkbox_value = select_lesson.internal_lesson_id == current_internal_lesson && select_lesson.merged_lesson_id == current_merged_lesson;
                        is_visible = is_visible || checkbox_value;
                        const std::string checkbox_description = all_lessons[(first_mwday + select_day) % 7][current_merged_lesson].get_description(current_internal_lesson);
                        if (ImGui::Checkbox(checkbox_description.c_str(), &checkbox_value) && checkbox_value) 
                        {
                            is_visible = true;
                            select_lesson.internal_lesson_id = current_internal_lesson;
                            select_lesson.merged_lesson_id = current_merged_lesson;
                        }
                    }
                }
            }
            if (!is_visible) select_lesson = {-1, -1};
        }
        ImGui::EndGroup();
        if (ImGui::Button("OK") && is_ok_possible(is_calendar_filled)) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена")) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Add_Working_Out::accept_changes(std::vector<Calendar_Day&>& all_days)
{
    IM_ASSERT(check_ok());
    
    //lesson_to_workout.lesson_pair = all_lessons[(first_mwday + select_day) % 7][select_lesson.merged_lesson_id].get_lesson_pair(select_lesson.internal_lesson_id);
    std::tm date_to = { 0, 0, 0, // second, minute, hour
    select_day + 1, select_month, select_year}; // 1-based day, 0-based month, year since 1900 
    date_to.tm_wday = get_wday(select_day, select_month, select_year);
    Workout_Info lesson_to_workout { all_students[select_student], all_lessons[(first_mwday + select_day) % 7][select_lesson.merged_lesson_id], select_lesson.internal_lesson_id, date_to };
    all_days[caller_mday].add_workout(all_students[select_student], caller_lesson, lesson_to_workout);//[popup_add_working_out_merged_lesson][popup_add_working_out_internal_lesson]
    all_days[select_day].set_status(select_lesson, select_student, STATUS_WORKED_OUT);
    int current_student_contract = all_students[select_student].get_contract();
    int current_discount_level = -1;
    for (int i = 0; i < all_students.size(); i++)
    {
        if (all_students[i].get_contract() == current_student_contract && current_discount_level < 2 && !all_students[i].is_removed())
        {
            current_discount_level++;
        }
    }
    if (current_discount_level == -1) current_discount_level = 0;
    int current_internal_student_id = all_days[select_day].find_student(all_students[select_student], select_lesson.merged_lesson_id);
    all_days[select_day].set_discount_status(select_lesson, current_internal_student_id, current_discount_level);
}