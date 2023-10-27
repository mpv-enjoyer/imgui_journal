#include "add_working_out.h"

Popup_Add_Working_Out::Popup_Add_Working_Out(const std::vector<Student*>& all_students, std::vector<std::vector<Lesson_Info*>>& all_lessons, const std::vector<Calendar_Day*>& all_days,
Group& current_group, const std::tm& current_time, const std::tm& current_lesson_time, Lesson current_lesson):
all_students(all_students), all_lessons(all_lessons), all_days(all_days), current_group(current_group)
{
    for (int i = 0; i < all_students.size(); i++)
    {
        if (all_students[i]->is_removed()) continue;
        if (current_group.is_in_group(PTRREF(all_students[i]))) continue;
        possible_student_descriptions.push_back((all_students[i]->get_name() + " (" + std::to_string(all_students[i]->get_contract()) + ")"));
        possible_student_ids.push_back(i);
    }
    first_mwday = calculate_first_mwday(current_time.tm_mday, current_time.tm_wday);
    count_mday = get_number_of_days(current_time.tm_mon, current_time.tm_year + 1900);
    select_month = current_time.tm_mon;
    select_year = current_time.tm_year;
    caller_lesson = current_lesson;
    caller_mday = current_lesson_time.tm_mday;
    caller_month = current_lesson_time.tm_mon;
    caller_year = current_lesson_time.tm_year;
    int caller_wday = get_wday(caller_mday, caller_month, caller_year);
    caller_lesson_name_id = all_lessons[caller_wday][current_lesson.merged_lesson_id]->get_lesson_pair(current_lesson.internal_lesson_id).lesson_name_id;
}

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
                        for (int k = 0; k < all_lessons[(first_mwday + i) % 7][j]->get_lessons_size(); k++)
                        {
                            bool new_attend = all_lessons[(first_mwday + i) % 7][j]->should_attend(PTRREF(all_students[select_student]));
                            new_attend = new_attend && all_lessons[(first_mwday + i) % 7][j]->get_lesson_pair(k).lesson_name_id == caller_lesson_name_id;
                            should_attend = should_attend || new_attend;
                        }
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
                Lesson_Info& current_select_lesson_ref = PTRREF(all_lessons[(first_mwday + select_day) % 7][current_merged_lesson]);
                for (int current_internal_lesson = 0; current_internal_lesson < current_select_lesson_ref.get_lessons_size(); current_internal_lesson++)
                {
                    current_lesson.internal_lesson_id = current_internal_lesson;
                    if (select_month == caller_month && select_year == caller_year && select_day == caller_mday && current_lesson == caller_lesson) continue;
                    if (current_select_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id != caller_lesson_name_id) continue;
                    if (current_select_lesson_ref.should_attend(PTRREF(all_students[select_student])))
                    {
                        bool checkbox_value = select_lesson.internal_lesson_id == current_internal_lesson && select_lesson.merged_lesson_id == current_merged_lesson;
                        is_visible = is_visible || checkbox_value;
                        const std::string checkbox_description = all_lessons[(first_mwday + select_day) % 7][current_merged_lesson]->get_description(current_internal_lesson);
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

bool Popup_Add_Working_Out::is_ok_possible(bool is_calendar_filled) 
{
    if (select_student == -1) { error("Выберите ученика"); return false; }
    if (!is_calendar_filled) {error("Для выбранного ученика нет доступных отработок"); return false; }
    if (select_day == -1) { error("Выберите день"); return false; }
    if (select_lesson == Lesson {-1, -1}) { error("Выберите урок"); return false; }
    int student_id = possible_student_ids[select_student];
    int known_internal_student_id = all_days[select_day]->find_student(PTRREF(all_students[student_id]), select_lesson.merged_lesson_id);
    Student_Status requested_status = all_days[select_day]->get_status(select_lesson, known_internal_student_id);
    if (requested_status.status == STATUS_WORKED_OUT) { error("Отработка уже назначена"); return false; }
    if (requested_status.status == STATUS_NOT_AWAITED) { error("Ученик не должен приходить на этот урок"); return false; }
    if (requested_status.status == STATUS_ON_LESSON) { error("Ученик присутствовал на этом уроке"); return false; }
    if (requested_status.status == STATUS_INVALID) { error("SYSTEM_STATUS_INVALID"); return false; }
    return true;
};

void Popup_Add_Working_Out::accept_changes(std::vector<Calendar_Day*>& all_days)
{
    IM_ASSERT(check_ok());
    std::tm date_to = { 0, 0, 0, // second, minute, hour
    select_day + 1, select_month, select_year}; // 1-based day, 0-based month, year since 1900 
    date_to.tm_wday = get_wday(select_day, select_month, select_year);
    int student_id = possible_student_ids[select_student];
    Student& current_student = PTRREF(all_students[student_id]);
    int known_internal_student_id = all_days[select_day]->find_student(PTRREF(all_students[student_id]), select_lesson.merged_lesson_id);
    Workout_Info lesson_to_workout { all_students[student_id], all_lessons[(first_mwday + select_day) % 7][select_lesson.merged_lesson_id], select_lesson.internal_lesson_id, date_to };
    all_days[caller_mday]->add_workout(current_student, caller_lesson, lesson_to_workout);
    all_days[select_day]->set_status(select_lesson, known_internal_student_id, STATUS_WORKED_OUT);
    int current_student_contract = all_students[student_id]->get_contract();
    int current_discount_level = -1;
    for (int i = 0; i < all_students.size(); i++)
    {
        if (all_students[i]->get_contract() == current_student_contract && current_discount_level < 2 && !(all_students[i]->is_removed()))
        {
            current_discount_level++;
        }
    }
    if (current_discount_level == -1) current_discount_level = 0;
    //
    all_days[select_day]->set_discount_status(select_lesson, known_internal_student_id, current_discount_level);
}