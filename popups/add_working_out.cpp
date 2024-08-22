#include "add_working_out.h"

Popup_Add_Working_Out::Popup_Add_Working_Out(Graphical* _graphical, const std::tm current_lesson_time, Lesson current_lesson, const Lesson_Info* current_lesson_info)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    main_journal = journal->journal_main ? journal->journal_main : journal;
    current_journal = journal;
    caller_lesson_info = current_lesson_info;
    current_group = &(caller_lesson_info->get_group());
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
    for (int i = 0; i < journal->student_count(); i++)
    {
        if (journal->student(i)->is_removed()) continue;
        int internal_student_id = current_group->find_student(PTRREF(journal->student(i)));
        if (internal_student_id != -1 && !current_group->is_deleted(PTRREF(journal->student(i))))
        {
            if (current_lesson_info->should_attend(internal_student_id, current_lesson.internal_lesson_id)) continue;;
        }

        bool was_found = false;
        std::vector<int> student_ids;
        int wday = graphical->wday;
        int cell_index = get_mday_index_for_wday(current_lesson_time.tm_mday, wday, current_lesson_time.tm_mon, current_lesson_time.tm_year);
        const std::vector<std::vector<const Workout_Info_ *>> caller_lesson_workout_info = journal->get_workout_info(wday, current_lesson, &student_ids);
        for (int j = 0; j < student_ids.size(); j++)
        {
            if (student_ids[j] == i && caller_lesson_workout_info[j][cell_index] != nullptr) // Current student already working out on that lesson?
            {
                was_found = true;
                break;
            }
        }
        if (was_found) continue;
        possible_student_descriptions.push_back((journal->student(i)->get_name() + " (" + std::to_string(journal->student(i)->get_contract()) + ")"));
        possible_student_ids.push_back(i);
    }
    picker = Picker(possible_student_descriptions, possible_student_ids);
    if (!possible_student_descriptions.size()) quit_early = true;
    first_mwday = get_first_mwday(journal->current_month(), journal->current_year());
    count_mday = get_number_of_days(journal->current_month(), journal->current_year() + 1900);
    select_month = journal->current_month();
    select_year = journal->current_year();
    caller_lesson = current_lesson;
    caller_mday = current_lesson_time.tm_mday;
    caller_month = current_lesson_time.tm_mon;
    caller_year = current_lesson_time.tm_year;
    int caller_wday = graphical->wday;
    caller_lesson_name_id = journal->lesson_info(caller_wday, current_lesson.merged_lesson_id)->get_lesson_pair(current_lesson.internal_lesson_id).lesson_name_id;
}

void Popup_Add_Working_Out::update_possible_lessons()
{
    possible_lessons.clear();
    possible_lessons = std::vector<std::vector<Lesson>>(current_journal->day_count());
    std::tm input_date = { 0, 0, 0, 
        0, select_month, select_year};
    //if (select_student == -1) return;
    Lesson_Pair caller_pair = caller_lesson_info->get_lesson_pair(caller_lesson.internal_lesson_id);
    int caller_lesson_type = caller_pair.lesson_name_id;
    for (int i = 0; i < current_journal->day_count(); i++)
    {
        int wday = current_journal->wday(i);
        input_date.tm_mday = i;
        input_date.tm_wday = wday;
        for (int j = 0; j < current_journal->lesson_info_count(wday); j++)
        {
            for (int k = 0; k < current_journal->lesson_info(wday, j)->get_lessons_size(); k++)
            {
                if (current_journal->is_workout_possible(current_journal->lesson_info(wday, j), k, select_student, caller_lesson_type))
                {
                    possible_lessons[i].push_back({j, k});
                }
            }
        }
    }
}

void Popup_Add_Working_Out::update_journal(int month, int year)
{
    bool dealloc_current_month = current_journal != journal && current_journal != main_journal;
    bool is_changed_month_main = main_journal->current_month() == month && main_journal->current_year() == year;
    bool is_changed_month_caller = journal->current_month() == month && journal->current_year() == year;

    if (dealloc_current_month)
    {
        current_journal->restrict_saving = true;
        delete current_journal;
    }

    if (is_changed_month_main)
    {
        current_journal = main_journal;
    }
    else if (is_changed_month_caller)
    {
        current_journal = journal;
    }
    else
    {
        current_journal = new Journal(month, year, journal);
    }
    first_mwday = get_first_mwday(current_journal->current_month(), current_journal->current_year());
    count_mday = get_number_of_days(current_journal->current_month(), current_journal->current_year() + 1900);
    select_student = -1;
    select_day = -1;
    select_month = month;
    select_year = year;
    select_lesson = {-1, -1};
    update_possible_lessons();
}

bool Popup_Add_Working_Out::show_frame()
{
    if (quit_early)
    {
        cancel();
        return true;
    }
    if (begin_frame("Добавление отработки"))
    {
        ImGui::BeginGroup();
        int result = picker.show();
        ImGui::EndGroup();
        if (result != select_student)
        {
            select_student = result;
            update_possible_lessons();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        if (ImGui::Button("<##workout"))
        {
            int month = select_month;
            int year = select_year;
            previous_month_for(month, year);
            update_journal(month, year);
        }; 
        ImGui::SameLine();
        ImGui::Button(current_journal->Month_name(current_journal->current_month()).c_str());
        ImGui::SameLine();
        if (ImGui::Button(">##workout"))
        {
            int month = select_month;
            int year = select_year;
            next_month_for(month, year);
            update_journal(month, year);
        };
        bool select_student_visible = result != -1;
        if (!select_student_visible)
        {
            ImGui::BeginDisabled();
        }
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
                    if (j_button_selectable(std::to_string(i + 1).c_str(), current_selected, true))
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
        ImGui::EndTable();
        if (!select_student_visible)
        {
            ImGui::EndDisabled();
        }
        if (select_day != -1)
        {
            int select_mday = current_journal->wday(select_day);
            bool select_lesson_shown = false;
            for (int i = 0; i < possible_lessons[select_day].size(); i++)
            {
                const Lesson_Info* current = current_journal->lesson_info(select_mday, possible_lessons[select_day][i].merged_lesson_id);
                std::string description = current->get_description(possible_lessons[select_day][i].internal_lesson_id);
                bool checkbox_value = select_lesson == possible_lessons[select_day][i];
                if (ImGui::Checkbox(description.c_str(), &checkbox_value))
                {
                    select_lesson = possible_lessons[select_day][i];
                }
                select_lesson_shown |= checkbox_value;
            }
            if (!select_lesson_shown)
                select_lesson = {-1, -1};
        }
        ImGui::EndGroup();
        if (ImGui::Button("OK") && is_ok_possible(is_calendar_filled)) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

bool Popup_Add_Working_Out::is_ok_possible(bool is_calendar_filled) 
{
    if (select_student == -1) { error("Выберите ученика"); return false; }
    if (!is_calendar_filled) { error("Для выбранного ученика нет доступных отработок"); return false; }
    if (select_day == -1) { error("Выберите день"); return false; }
    if (select_lesson == Lesson {-1, -1}) { error("Выберите урок"); return false; }
    int student_id = select_student;
    int wday = current_journal->wday(select_day);
    const Lesson_Info& select_lesson_info = PTRREF(current_journal->lesson_info(wday, select_lesson.merged_lesson_id));
    const Student& student = PTRREF(current_journal->student(student_id));
    Student_Status requested_status = current_journal->day(select_day)->get_status(select_lesson_info, select_lesson.internal_lesson_id, student);
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
    // Assume that students aren't being deleted
    current_journal->add_working_out(caller_date, select_date, select_student, caller_lesson, select_lesson );
}

Popup_Add_Working_Out::~Popup_Add_Working_Out()
{
    if (journal != main_journal)
    {
        if (current_journal == journal)
        {
            current_journal->save_workouts();
            main_journal->load_workouts();
            return;
        }
        if (current_journal == main_journal)
        {
            current_journal->save_workouts();
            journal->load_workouts();
            return;
        }
        delete current_journal;
        journal->load_workouts();
        return;
    }

    // Caller is current month
    if (main_journal != current_journal)
    {
        delete current_journal;
        // Destructor saved new workout to the list so we need to load the list again
        main_journal->load_workouts();
    }
}

Popup_Add_Working_Out::Picker::Picker(std::vector<std::string> descriptions, std::vector<int> id_list) 
: _descriptions(descriptions), _id_list(id_list)
{
    if (id_list.size() != 0) use_id_list = true;
    if (id_list.size() != descriptions.size())
        throw std::invalid_argument("id list size != descriptions size");
}

int Popup_Add_Working_Out::Picker::show()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f));
    filter.Draw("Поиск по ученикам");
    ImGui::PopStyleColor(1);
    ImGui::BeginChild("Child window", ImVec2(500,300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < _descriptions.size(); i++)
    {
        if (!filter.PassFilter(_descriptions[i].c_str())) continue;
        
        if (current == i)
        {
            std::string student_button_name = generate_label("Выбран.##", { i });
            j_button_selectable(student_button_name.c_str(), true);
        }
        else
        {
            std::string student_button_name = generate_label("Выбрать##", { i });
            if (ImGui::Button(student_button_name.c_str())) 
            {
                current = i;
            }
        }
        ImGui::SameLine();
        ImGui::Text(_descriptions[i].c_str());
    }
    ImGui::EndChild();
    if (use_id_list && current != -1) return _id_list[current];
    else return current;
}