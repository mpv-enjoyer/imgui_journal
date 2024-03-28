#include "render.h"

/*Elements::Picker::Picker(std::vector<std::string> descriptions, std::vector<int> id_list) 
: _descriptions(descriptions), _id_list(id_list)
{
    if (id_list.size() != 0) use_id_list = true;
    if (id_list.size() != descriptions.size())
        throw std::invalid_argument("id list size != descriptions size");
}

int Elements::Picker::show()
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
    if (use_id_list) return _id_list[current];
    else return current;
}*/

void Elements::table(int merged_lesson_id)
{
    const Lesson_Info& merged_lesson = PTRREF(Journal::lesson_info(Graphical::wday(), merged_lesson_id));
    if (!Graphical::is_edit_mode() && merged_lesson.is_discontinued()) return;
    bool disabled = merged_lesson.is_discontinued();
    if (disabled) ImGui::BeginDisabled();
    if (merged_lesson_id != 0 && Journal::lesson_info(Graphical::wday(), merged_lesson_id - 1)->get_lesson_pair(0).time_begin == merged_lesson.get_lesson_pair(0).time_begin)
    {
        ImGui::SameLine();
    }
    ImGui::BeginGroup();
    if (disabled)
    {
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Удаленная"); ImGui::SameLine();
    }
    ImGui::Text(merged_lesson.get_description().c_str());
    std::string table_name = generate_label("##table", { merged_lesson_id });
    int table_columns = DEFAULT_COLUMN_COUNT + Graphical::visible_days().size() + 1;
    if (ImGui::BeginTable(table_name.c_str(), table_columns, 
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoPadInnerX,
    ImVec2(std::numeric_limits<float>::max(),(0.0F))))
    {
        ImU32 row_bg_color = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
        ImGui::TableNextColumn(); ImGui::Text("No");
        ImGui::TableNextColumn(); ImGui::Text("ФИ ученика");
        ImGui::TableNextColumn(); ImGui::Text("Д-р");
        ImGui::TableNextColumn(); ImGui::Text("Программа");
        ImGui::TableNextColumn(); ImGui::Text("Цена");
        for (int i = 0; i < Graphical::visible_days().size(); i++)
        {
            std::string day_temp = std::to_string(Graphical::visible_days()[i].number) + "." + std::to_string(Journal::current_month() + 1);
            ImGui::TableNextColumn(); 
            ImGui::Text(day_temp.c_str());
        }
        ImGui::TableNextColumn();
        ImGui::Text("Сумма");
        int counter = 1;
        for (int internal_student_id = 0; internal_student_id < merged_lesson.get_group().get_size(); internal_student_id++)
        {
            bool enabled = table_row(merged_lesson_id, internal_student_id, counter);
            if (enabled) counter++;
        }
        table_add_student_row(merged_lesson_id, counter);
        table_add_workout_row(merged_lesson_id, counter);
        ImGui::EndTable();
        ImGui::EndGroup();
    }
    if (disabled) ImGui::EndDisabled();
}

bool Elements::table_row(int merged_lesson_id, int internal_student_id, int counter)
{
    const Lesson_Info& merged_lesson = PTRREF(Journal::lesson_info(Graphical::wday(), merged_lesson_id));
    const Group& group = merged_lesson.get_group();
    const Student& student = group.get_student(internal_student_id);
    const bool disabled = student.is_removed() || group.is_deleted(student);
    const int contract = student.get_contract();
    if (disabled && !Graphical::is_edit_mode()) return false;
    ImGui::TableNextRow();
    if (disabled) 
    {
        ImGui::BeginDisabled();
        ImGui::TableSetColumnIndex(0); ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),"-");
    }
    else
    {
        ImGui::TableSetColumnIndex(0); ImGui::Text("%i", counter);
    }
    ImGui::TableSetColumnIndex(1); ImGui::Text(student.get_name().c_str());
    ImGui::TableSetColumnIndex(2); ImGui::Text("%i", student.get_contract());
    Attend_Data attend_data = group.get_attend_data(internal_student_id);
    std::vector<bool> is_internal_lesson_enabled;
    if (merged_lesson.get_lessons_size() == 1)
        is_internal_lesson_enabled.push_back(true);
    else
    {
        if (attend_data == ATTEND_FIRST) is_internal_lesson_enabled = {true, false};
        if (attend_data == ATTEND_SECOND) is_internal_lesson_enabled = {false, true};
        if (attend_data == ATTEND_BOTH) is_internal_lesson_enabled = {true, true};
    }
    int show_price_sum = 0;
    std::string show_lesson_name = Journal::merged_lesson_name(Graphical::wday(), merged_lesson_id, internal_student_id);
    for (int i = 0; i < merged_lesson.get_lessons_size(); i++)
    {
        if (!is_internal_lesson_enabled[i]) continue;
        int lesson_type = merged_lesson.get_lesson_pair(i).lesson_name_id;
        show_price_sum += Journal::lesson_common_price(contract, lesson_type);
    }
    ImGui::TableSetColumnIndex(3);
    ImGui::Text(show_lesson_name.c_str());
    ImGui::TableSetColumnIndex(4); ImGui::Text(c_str_int(show_price_sum));

    int price_sum = 0;
    for (int current_day_cell = 0; current_day_cell < Graphical::visible_days().size(); current_day_cell++)
    {
        price_sum += table_cell(merged_lesson_id, internal_student_id, current_day_cell);
    }
    ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + Graphical::visible_days().size());
    ImGui::TextDisabled(c_str_int(price_sum));
    if (disabled) ImGui::EndDisabled();
    return !disabled;
}

int Elements::table_cell(int merged_lesson_id, int internal_student_id, int visible_day_id)
{
    ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + visible_day_id);
    auto visible_day = Graphical::visible_days()[visible_day_id];
    bool enabled = visible_day.is_today && !Graphical::is_edit_mode();
    enabled |= (!visible_day.is_future && Graphical::is_edit_mode());
    if (!enabled) ImGui::BeginDisabled();
    const Calendar_Day* day = visible_day.day;
    const int mday = visible_day.number - MDAY_DIFF;
    const int wday = Journal::wday(mday);
    const Lesson_Info* merged_lesson = Journal::lesson_info(wday, merged_lesson_id);
    int price_sum = 0;
    for (int internal_lesson = 0; internal_lesson < merged_lesson->get_lessons_size(); internal_lesson++)
    {
        Lesson lesson;
        lesson.merged_lesson_id = merged_lesson_id;
        lesson.internal_lesson_id = internal_lesson;
        Student_Status status = day->get_status(lesson, internal_student_id);
        ImGui::BeginGroup();
        std::string combo_attendance_name = generate_label("##combo_attendance", {merged_lesson_id, internal_lesson, visible_day_id, internal_student_id});
        std::string tooltip = "";
        bool workout_exists = false;
        if (status.status == STATUS_WORKED_OUT)
        {
            workout_exists = true;
            auto current_workout_info = day->get_status(lesson, internal_student_id).workout_info;
            tooltip = to_string(current_workout_info.date, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_begin, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_end);
        }
        if (Elements::attendance_combo(combo_attendance_name.c_str(), &(status.status), tooltip))
        {
            Journal::set_lesson_status(mday, lesson, internal_student_id, status, workout_exists);
        }
        int price = Journal::lesson_current_price(lesson, mday, internal_student_id);
        if (price != -1)
        {
            price_sum += price;
            ImGui::TextDisabled(std::to_string(price).c_str());
        }
        else 
        {
            const Student& student = merged_lesson->get_group().get_student(internal_student_id);
            const int lesson_type = merged_lesson->get_lesson_pair(internal_lesson).lesson_name_id;
            price_sum += Journal::lesson_common_price(student.get_contract(), lesson_type);
        }
        ImGui::EndGroup();
        ImGui::SameLine(0.0f, 2.0f);
    }
    if (!enabled) ImGui::EndDisabled();
    return price_sum;
}

void Elements::table_add_student_row(int merged_lesson_id, int counter)
{
    const Lesson_Info& merged_lesson = PTRREF(Journal::lesson_info(Graphical::wday(), merged_lesson_id));
    const Group& group = merged_lesson.get_group();
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextDisabled(c_str_int(counter));
    ImGui::TableSetColumnIndex(1);
    std::string add_student_button_name = generate_label("Добавить ученика##", {merged_lesson_id});
    if (ImGui::Button(add_student_button_name.c_str()))
    {
        Graphical::popup_add_student_to_group() = new Popup_Add_Student_To_Group(merged_lesson, merged_lesson_id, Graphical::wday());
    }
}

void Elements::table_add_workout_row(int merged_lesson_id, int counter)
{
    const Lesson_Info& merged_lesson = PTRREF(Journal::lesson_info(Graphical::wday(), merged_lesson_id));
    const Group& group = merged_lesson.get_group();
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextDisabled(c_str_int(counter));
    ImGui::TableSetColumnIndex(1);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Отр.:"); ImGui::SameLine();
    bool disabled = Journal::current_time.tm_wday != Graphical::wday();
    if (disabled) ImGui::BeginDisabled();
    std::string first_lesson_name = Journal::Lesson_name(merged_lesson.get_lesson_pair(0).lesson_name_id);
    std::string second_lesson_name;
    if (merged_lesson.get_lessons_size() == 2)
        second_lesson_name = Journal::Lesson_name(merged_lesson.get_lesson_pair(1).lesson_name_id);
    if (!Graphical::is_edit_mode() && ImGui::Button(first_lesson_name.c_str()))
    {
        std::tm current_lesson_time = { 0, 0, 0, 
        Journal::current_time.tm_mday - MDAY_DIFF, Journal::current_month(), Journal::current_year() };
        Lesson lesson = {merged_lesson_id, 0};
        Graphical::popup_add_working_out() = new Popup_Add_Working_Out(current_lesson_time, lesson, &merged_lesson);
    }
    ImGui::SameLine(0.0f, 2.0f);
    if (!Graphical::is_edit_mode() && merged_lesson.get_lessons_size() == 2 && ImGui::Button(second_lesson_name.c_str()))
    {
        std::tm current_lesson_time = { 0, 0, 0, 
        Journal::current_time.tm_mday - MDAY_DIFF, Journal::current_month(), Journal::current_year() };
        Lesson lesson = {merged_lesson_id, 1};
        Graphical::popup_add_working_out() = new Popup_Add_Working_Out(current_lesson_time, lesson, &merged_lesson);
    } 
    if (disabled) ImGui::EndDisabled();

    std::vector<const Student*> workout_students;
    for (int day_id = 0; day_id < Graphical::visible_days().size(); day_id++)
    {
        ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + day_id);
        for (int internal_lesson = 0; internal_lesson < merged_lesson.get_lessons_size(); internal_lesson++)
        {
            Lesson lesson;
            lesson.merged_lesson_id = merged_lesson_id;
            lesson.internal_lesson_id = internal_lesson;
            Day_With_Info visible_day = Graphical::visible_days()[day_id];
            Journal::append_workout_students(visible_day, lesson, workout_students);
            if (!Graphical::is_edit_mode()) continue;
            if (Graphical::visible_days()[day_id].is_future) continue;
            if (internal_lesson != 0) ImGui::SameLine(0.0f, 2.0f);
            std::string add_workout_button_name = generate_label("Отр##add_workout_button", {merged_lesson_id, day_id, internal_lesson});
            if (ImGui::Button(add_workout_button_name.c_str(), ImVec2(SUBCOLUMN_WIDTH_PXLS, 0.0f)))
            {
                std::tm current_lesson_time = { 0, 0, 0,
                        Graphical::visible_days()[day_id].number - MDAY_DIFF, Journal::current_month(), Journal::current_year() };
                Graphical::popup_add_working_out() = new Popup_Add_Working_Out(current_lesson_time, lesson, &merged_lesson);
            }
        }
    }

    for (int workout_student_id = 0; workout_student_id < workout_students.size(); workout_student_id++)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text(c_str_int(counter));
        ImGui::TableSetColumnIndex(1);
        ImGui::Text(workout_students[workout_student_id]->get_name().c_str());
        for (int day_id = 0; day_id < Graphical::visible_days().size(); day_id++)
        {
            bool first_present = false;
            for (int internal_lesson = 0; internal_lesson < merged_lesson.get_lessons_size(); internal_lesson++)
            {
                Lesson lesson;
                lesson.merged_lesson_id = merged_lesson_id;
                lesson.internal_lesson_id = internal_lesson;
                if (internal_lesson == 0) first_present = true;
                lesson.internal_lesson_id = internal_lesson;
                ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + day_id);
                Workout_Info current_workout_info = Graphical::visible_days()[day_id].day->get_workout_info(lesson, PTRREF(workout_students[workout_student_id]));
                if (current_workout_info.internal_lesson == -1) continue;
                if (internal_lesson == 0) first_present = false;
                bool create_fake_radio = internal_lesson != 0 && !first_present;
                std::string workout_info_radio_tooltip_name = generate_label("##workout_info_radio_tooltip", {day_id, workout_student_id, internal_lesson, merged_lesson_id});
                if (create_fake_radio)
                {
                    std::string fake_radio_name = generate_label("##fake_radio", {day_id, 0, internal_lesson, -1});
                    ImGui::InvisibleButton(fake_radio_name.c_str(), ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight()));
                };
                bool dummy = true;
                ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
                ImGui::SameLine();
                if (ImGui::Checkbox(workout_info_radio_tooltip_name.c_str(), &dummy))
                {
                    if (current_workout_info.date.tm_mon != Journal::current_time.tm_mon) throw std::invalid_argument("not implemented");
                    int internal_student_id = group.find_student(PTRREF(current_workout_info.student));
                    Student_Status new_status;
                    new_status.discount_status = -1;
                    new_status.status = STATUS_NO_DATA;
                    Journal::set_lesson_status(current_workout_info.date.tm_mday - MDAY_DIFF, lesson, internal_student_id, new_status, true);
                }
                ImGui::SetItemTooltip(to_string(current_workout_info.date, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_begin, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_end).c_str());
            }
        }
    }
}
