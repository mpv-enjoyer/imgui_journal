#include "add_working_out.h"

Popup_Add_Working_Out::Popup_Add_Working_Out(const std::tm& current_time, const std::tm& current_lesson_time, Lesson current_lesson, Lesson_Info* current_lesson_info)
{
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
    for (int i = 0; i < Journal::student_count(); i++)
    {
        if (Journal::student(i)->is_removed()) continue;
        if (current_group.is_in_group(PTRREF(Journal::student(i))) && !current_group.is_deleted(PTRREF(Journal::student(i)))) continue;
        bool was_found = false;
        for (int j = 0; j < Journal::day(current_lesson_time.tm_mday)->get_workout_size(current_lesson); j++)
        {
            const Student* student = Journal::day(current_lesson_time.tm_mday)->get_workout_student(current_lesson, j);
            if (Journal::student(i) == student) was_found = true;
        }
        if (was_found) continue;
        possible_student_descriptions.push_back((Journal::student(i)->get_name() + " (" + std::to_string(Journal::student(i)->get_contract()) + ")"));
        possible_student_ids.push_back(i);
    }
    picker = Elements::Picker(possible_student_descriptions, possible_student_ids);
    if (!possible_student_descriptions.size()) quit_early = true;

    first_mwday = calculate_first_mwday(current_time.tm_mday, current_time.tm_wday);
    count_mday = get_number_of_days(current_time.tm_mon, current_time.tm_year + 1900);
    select_month = current_time.tm_mon;
    select_year = current_time.tm_year;
    caller_lesson = current_lesson;
    caller_mday = current_lesson_time.tm_mday;
    caller_month = current_lesson_time.tm_mon;
    caller_year = current_lesson_time.tm_year;
    int caller_wday = get_wday(caller_mday, caller_month, caller_year);
    caller_lesson_name_id = Journal::lesson_info(caller_wday, current_lesson.merged_lesson_id)->get_lesson_pair(current_lesson.internal_lesson_id).lesson_name_id;
    caller_lesson_info = current_lesson_info;
}

void Popup_Add_Working_Out::update_possible_lessons()
{
    possible_lessons.clear();
    possible_lessons = std::vector<std::vector<Lesson>>(Journal::day_count());
    std::tm input_date = { 0, 0, 0, 
        0, select_month, select_year};
    Lesson_Pair caller_pair = caller_lesson_info->get_lesson_pair(caller_lesson.internal_lesson_id);
    int caller_lesson_type = caller_pair.lesson_name_id;
    for (int i = 0; i < Journal::day_count(); i++)
    {
        int wday = Journal::wday(i);
        input_date.tm_mday = i;
        input_date.tm_wday = wday;
        for (int j = 0; j < Journal::lesson_info_count(wday); j++)
        {
            for (int k = 0; k < Journal::lesson_info(wday, j)->get_lessons_size(); k++)
            {
                if (Journal::is_workout_possible(Journal::lesson_info(wday, j), k, select_student, caller_lesson_type))
                {
                    possible_lessons[i].push_back({j, k});
                }
            }
        }
    }
}

bool Popup_Add_Working_Out::show_frame()
{
    if (quit_early)
    {
        cancel();
        return true;
    }
    POPUP_INIT_FRAME("Добавление отработки")
    {
        int result = picker.show();
        if (result != select_student)
        {
            update_possible_lessons();
        }
        bool select_student_visible = result != -1;
        if (!select_student_visible)
        {
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
            std::tm select_date = { 0, 0, 0, 
                    0, select_month, select_year};
            
            for (int i = 0; i < possible_lessons.size(); i++)
            {
                if (possible_lessons[i].size() > 0)
                {
                    is_calendar_filled = true;
                    bool current_selected = select_day == i;
                    if (Elements::button_selectable(std::to_string(i + 1).c_str(), current_selected, true))
                    {
                        if (current_selected) select_day = -1;
                        else select_day = i;
                    }
                }
                else
                {
                    if (select_day == i) select_day = -1;
                    ImGui::BeginDisabled();
                    ImGui::SmallButton(c_str_int(i + 1));
                    ImGui::EndDisabled();
                }
                ImGui::TableNextColumn();
            }
        }
        if (!select_student_visible)
        {
            ImGui::EndDisabled();
        }
        if (select_day != -1)
        {
            int select_mday = Journal::wday(select_day);
            bool select_lesson_shown = false;
            for (int i = 0; i < possible_lessons[select_day].size(); i++)
            {
                const Lesson_Info* current = Journal::lesson_info(select_mday, possible_lessons[select_day][i].merged_lesson_id);
                std::string description = current->get_description(i);
                bool checkbox_value = select_lesson == possible_lessons[select_day][i];
                select_lesson_shown |= checkbox_value;
                if (ImGui::Checkbox(description.c_str(), &checkbox_value))
                {
                    select_lesson = possible_lessons[select_day][i];
                }
            }
            if (!select_lesson_shown)
                select_lesson = {-1, -1};
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
    int student_id = select_student;
    int wday = Journal::wday(select_day);
    const Lesson_Info& select_lesson_info = PTRREF(Journal::lesson_info(wday, select_lesson.merged_lesson_id));
    const Student& student = PTRREF(Journal::student(student_id));
    Student_Status requested_status =  Journal::day(select_day)->get_status(select_lesson_info, select_lesson.internal_lesson_id, student);
    if (requested_status.status == STATUS_WORKED_OUT) { error("Отработка уже назначена"); return false; }
    if (requested_status.status == STATUS_NOT_AWAITED) { error("Ученик не должен приходить на этот урок"); return false; }
    if (requested_status.status == STATUS_ON_LESSON) { error("Ученик присутствовал на этом уроке"); return false; }
    if (requested_status.status == STATUS_INVALID) { error("SYSTEM_STATUS_INVALID"); return false; }
    return true;
};

void Popup_Add_Working_Out::accept_changes()
{
    IM_ASSERT(check_ok());
    std::tm select_date = { 0, 0, 0,
    select_day, select_month, select_year };
    std::tm caller_date = { 0, 0, 0,
    caller_mday, caller_month, caller_year };

    Journal::add_working_out(caller_date, select_date, select_student, caller_lesson, select_lesson );
}