#include "render.h"

void Render::show_frame()
{
glfwPollEvents(); //TODO: do not update the screen until needed. Must be pretty straightforward.
// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();
const ImGuiViewport* viewport = ImGui::GetMainViewport();
ImGui::SetNextWindowPos(viewport->WorkPos);
ImGui::SetNextWindowSize(viewport->WorkSize);

ImGui::Begin("Журнал версии 0.0.1", nullptr, WINDOW_FLAGS);

ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar;
ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
if(ImGui::Button("Изменить день"))
{
    glfwPostEmptyEvent();
    Graphical::popup_select_day_of_the_week = new Popup_Select_Day_Of_The_Week();
};
ImGui::SameLine();
if (ImGui::Button("Добавить группу"))
{
    Graphical::popup_add_merged_lesson_to_journal = new Popup_Add_Merged_Lesson_To_Journal(Graphical::wday());
}
ImGui::SameLine();
if (ImGui::Button("Ученики") )
{
    Graphical::subwindow_students_list = new Subwindow_Students_List(all_students, all_groups);
}
ImGui::SameLine();
if (ImGui::Button("Группы"))
{
    subwindow_lessons_list = new Subwindow_Lessons_List(all_lessons, all_days, current_month, current_year);
}
ImGui::SameLine();
ImGui::Button("Журнал оплаты");
ImGui::SameLine();
ImGui::Button("Справка");
ImGui::SameLine();
static bool edit_mode = false;
ImGui::Checkbox("Режим редактирования", &edit_mode);
ImGui::Text("Выбран день %s, %s текущего года", Day_Names[current_day_of_the_week].c_str(), Month_Names[current_month].c_str());

ImGui::BeginChild("Child", ImVec2(0, -20), true, window_flags);
JTime previous = {-1, -1};
Lesson current_lesson;

if (all_lessons[current_day_of_the_week].size() == 0) ImGui::Text("На текущий день не запланированы уроки.");
for (int current_merged_lesson = 0; current_merged_lesson < all_lessons[current_day_of_the_week].size(); current_merged_lesson++)
{
    Lesson_Info& current_merged_lesson_ref = PTRREF(all_lessons[current_day_of_the_week][current_merged_lesson]);
    if (!edit_mode && current_merged_lesson_ref.is_discontinued()) continue;
    bool disabled = current_merged_lesson_ref.is_discontinued();
    if (disabled) ImGui::BeginDisabled();
    if (previous == current_merged_lesson_ref.get_lesson_pair(0).time_begin)
    {
        ImGui::SameLine();
    }
    else
    {
        previous = current_merged_lesson_ref.get_lesson_pair(0).time_begin;
    }
    current_lesson.merged_lesson_id = current_merged_lesson;
    ImGui::BeginGroup();
    if (disabled)
    {
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Удаленная"); ImGui::SameLine();
    }
    ImGui::Text(current_merged_lesson_ref.get_description().c_str());
    std::string table_name = generate_label("##table", { current_merged_lesson, (int)all_lessons[current_day_of_the_week].size() });
    std::vector<std::string> current_lesson_names;
    std::vector<int> current_lesson_name_ids;
    for (int current_internal_lesson = 0; current_internal_lesson < current_merged_lesson_ref.get_lessons_size(); current_internal_lesson++)
    {
        current_lesson_names.push_back(Lesson_Names[current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id] + "##" + std::to_string(current_internal_lesson + 1));
        current_lesson_name_ids.push_back(current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id);
    }
    if (ImGui::BeginTable(table_name.c_str(), DEFAULT_COLUMN_COUNT+count_visible_days+1, 
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
        for (int i = 0; i < count_visible_days; i++)
        {
            std::string day_temp = std::to_string(visible_days[i].number) + "." + std::to_string(current_time.tm_mon + 1);
            ImGui::TableNextColumn(); 
            ImGui::Text(day_temp.c_str());
        }
        ImGui::TableNextColumn(); 
        ImGui::Text("Сумма");
        Group& current_group = current_merged_lesson_ref.get_group();
        int disabled_students_count = 0;
        for (int current_student_group_id = 0; current_student_group_id < current_group.get_size(); current_student_group_id++)
        {
            Student& current_student = current_group.get_student(current_student_group_id);
            bool disabled_student = current_student.is_removed() || current_group.is_deleted(current_student);
            disabled_students_count += disabled_student;
            if (disabled_student && !edit_mode) continue;
            ImGui::TableNextRow();
            if (disabled_student) 
            {
                ImGui::BeginDisabled();
                ImGui::TableSetColumnIndex(0); ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),"-");
            }
            else
            {
                ImGui::TableSetColumnIndex(0); ImGui::Text("%i", current_student_group_id + 1 - disabled_students_count);
            }
            ImGui::TableSetColumnIndex(1); ImGui::Text(current_student.get_name().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%i", current_student.get_contract());
            int current_student_contract = current_student.get_contract();
            int current_discount_level = -1;
            for (int i = 0; i < all_students.size(); i++)
            {
                if (all_students[i]->get_contract() == current_student_contract && current_discount_level < 2 && !(all_students[i]->is_removed()))
                {
                    current_discount_level++;
                }
            }
            if (current_discount_level == -1) current_discount_level = 0; //TODO: restructure
            std::vector<bool> is_relevant;
            for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson]->get_lessons_size(); current_internal_lesson++)
            {
                current_lesson.internal_lesson_id = current_internal_lesson;
                Attend_Data current_attend_data = current_merged_lesson_ref.get_group().get_attend_data(current_student_group_id);
                if ((current_internal_lesson == 0 && current_attend_data == ATTEND_SECOND) || (current_internal_lesson == 1 && current_attend_data == ATTEND_FIRST))
                {
                    is_relevant.push_back(false);
                }
                else
                {
                    is_relevant.push_back(true);
                }
            }
            int show_price = 0;
            int price_sum = 0;
            std::string show_lesson_names;
            bool temp_first = true;
            for (int i = 0; i < is_relevant.size(); i++)
            {
                if (is_relevant[i]) 
                {
                    if (temp_first)
                    {
                        temp_first = false;
                    }
                    else
                    {
                        show_lesson_names.append("+");
                    }
                    show_price += Lesson_Prices[current_merged_lesson_ref.get_lesson_pair(i).lesson_name_id][current_discount_level];
                    show_lesson_names.append(Lesson_Names[current_merged_lesson_ref.get_lesson_pair(i).lesson_name_id]);
                }
            }
            if (!temp_first)
            {
                ImGui::TableSetColumnIndex(3);
                if (edit_mode && current_merged_lesson_ref.get_lessons_size() == 2 && current_merged_lesson_ref.get_lesson_pair(0).lesson_name_id != current_merged_lesson_ref.get_lesson_pair(1).lesson_name_id)
                {
                    Attend_Data current_attend_data = current_merged_lesson_ref.get_group().get_attend_data(current_student_group_id);
                    std::string attend_data_combo_name = generate_label("##att data", { current_merged_lesson, current_student_group_id });
                    std::string first_lesson_name = Lesson_Names[current_merged_lesson_ref.get_lesson_pair(0).lesson_name_id];
                    std::string second_lesson_name = Lesson_Names[current_merged_lesson_ref.get_lesson_pair(1).lesson_name_id];
                    if (j_attend_data(attend_data_combo_name, &current_attend_data, first_lesson_name, second_lesson_name))
                    {
                        current_merged_lesson_ref.get_group().set_attend_data(current_student_group_id, current_attend_data);
                        fill_NAW_in_calendar(&visible_days, current_merged_lesson, current_attend_data, current_student_group_id);
                    }
                }
                else
                {
                    ImGui::Text(to_string(current_lesson_names, "+", is_relevant).c_str());
                }
                ImGui::TableSetColumnIndex(4); ImGui::Text(c_str_int(show_price));
            }
            for (int current_day_cell = 0; current_day_cell < count_visible_days; current_day_cell++)
            {
                ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);
                bool is_current_cell_enabled = (visible_days[current_day_cell].is_today) && !edit_mode;
                is_current_cell_enabled = is_current_cell_enabled || (!visible_days[current_day_cell].is_future && edit_mode);
                if (!is_current_cell_enabled) ImGui::BeginDisabled();
                Calendar_Day* current_day_cell_ref = visible_days[current_day_cell].day;
                for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson]->get_lessons_size(); current_internal_lesson++)
                {
                    current_lesson.internal_lesson_id = current_internal_lesson;
                    Student_Status current_status = current_day_cell_ref->get_status(current_lesson, current_student_group_id);
                    int current_lesson_discount_status = current_day_cell_ref->get_discount_status(current_lesson, current_student_group_id);
                    ImGui::BeginGroup();
                    std::string combo_attendance_name = generate_label("##combo_attendance", {current_merged_lesson, current_internal_lesson, current_day_cell, current_student_group_id});
                    std::string tooltip = "";
                    bool should_delete_distant_workout = false;
                    if (current_status.status == STATUS_WORKED_OUT)
                    {
                        should_delete_distant_workout = true;
                        auto current_workout_info = current_day_cell_ref->get_status(current_lesson, current_student_group_id).workout_info;
                        tooltip = to_string(current_workout_info.date, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_begin, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_end);
                    }
                    if (j_attendance_combo(combo_attendance_name.c_str(), &(current_status.status), tooltip))
                    {
                        if (current_status.status != STATUS_WORKED_OUT)
                        {
                            if (should_delete_distant_workout)
                            {
                                Workout_Info distant_workout_info = current_status.workout_info;
                                int day_index = current_status.workout_info.date.tm_mday - 1;
                                int internal_lesson_id = current_status.workout_info.internal_lesson;
                                all_days[day_index]->delete_workout(PTRREF(distant_workout_info.lesson_info), internal_lesson_id, PTRREF(distant_workout_info.student));
                            }
                            current_day_cell_ref->set_status(current_lesson, current_student_group_id, current_status.status);
                            current_day_cell_ref->set_discount_status(current_lesson, current_student_group_id, current_discount_level);
                        }
                        if (current_lesson_discount_status == -1) current_lesson_discount_status = current_discount_level;
                    }
                    int current_lesson_real_price = current_status.status > STATUS_NO_DATA ? Lesson_Prices[current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id][current_lesson_discount_status] : 0;
                    if ((current_lesson_discount_status >= 0 && current_discount_level != current_lesson_discount_status && current_status.status > STATUS_NO_DATA) || current_status.status == STATUS_WAS_ILL)
                    {
                        int current_lesson_price = -1;
                        if ((current_status.status == STATUS_ON_LESSON) || (current_status.status == STATUS_WORKED_OUT) || (current_status.status == STATUS_SKIPPED))
                        {
                            current_lesson_price = Lesson_Prices[current_merged_lesson_ref.get_lesson_pair(current_internal_lesson).lesson_name_id][current_lesson_discount_status];
                        }
                        if (current_status.status == STATUS_WAS_ILL)
                        {
                            current_lesson_price = 0;
                            current_lesson_real_price = 0;
                        }
                        if (current_lesson_price != -1)
                        {
                            ImGui::TextDisabled(std::to_string(current_lesson_price).c_str());
                        }
                    }
                    price_sum += current_lesson_real_price;
                    ImGui::EndGroup();
                    ImGui::SameLine(0.0f, 2.0f);
                }
                if (!is_current_cell_enabled) ImGui::EndDisabled();
            }
            ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + count_visible_days);
            ImGui::TextDisabled(c_str_int(price_sum));
            if (disabled_student) ImGui::EndDisabled();
        }
        int current_group_size = current_group.get_size() - disabled_students_count;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled(c_str_int(current_group_size + 1));
        ImGui::TableSetColumnIndex(1);
        std::string add_student_button_name = generate_label("Добавить ученика##", {current_merged_lesson});
        if (ImGui::Button(add_student_button_name.c_str()))
        {
            popup_add_student_to_group = new Popup_Add_Student_To_Group(current_merged_lesson_ref, all_students, current_lesson.merged_lesson_id);
        }
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled(c_str_int(current_group_size + 1));
        ImGui::TableSetColumnIndex(1);
        //std::string add_working_out_common_name = generate_label("Отр.:##", {})
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Отр.:"); ImGui::SameLine();
        if (current_time.tm_wday != current_day_of_the_week) ImGui::BeginDisabled();
        //std::string add_working_out_first = generate_label(, {});
        if (!edit_mode && ImGui::Button(current_lesson_names[0].c_str()))
        {
            std::tm current_lesson_time = { 0, 0, 0, current_time.tm_mday - 1, current_month, current_year };
            int current_name_id = current_lesson_name_ids[0];
            popup_add_working_out = new Popup_Add_Working_Out(all_students, all_lessons, all_days, current_group, current_time, current_lesson_time, {current_lesson.merged_lesson_id, 0}, &current_merged_lesson_ref);//, current_name_id);
        }
        ImGui::SameLine(0.0f, 2.0f);
        if (!edit_mode && current_lesson_names.size() == 2 && ImGui::Button(current_lesson_names[1].c_str()))
        {
            std::tm current_lesson_time = { 0, 0, 0, current_time.tm_mday - 1, current_month, current_year };
            int current_name_id = current_lesson_name_ids[1];
            popup_add_working_out = new Popup_Add_Working_Out(all_students, all_lessons, all_days, current_group, current_time, current_lesson_time, {current_lesson.merged_lesson_id, 1}, &current_merged_lesson_ref);//, current_name_id);
        } 
        if (current_time.tm_wday != current_day_of_the_week) ImGui::EndDisabled();
        std::vector<Student*> working_out_students;
        for (int current_day_cell = 0; current_day_cell < visible_days.size(); current_day_cell++)
        {
            Calendar_Day* current_day_cell_ref = visible_days[current_day_cell].day;
            for (int current_internal_lesson = 0; current_internal_lesson < current_merged_lesson_ref.get_lessons_size(); current_internal_lesson++)
            {
                current_lesson.internal_lesson_id = current_internal_lesson;
                ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);
                for (int workout_num = 0; workout_num < current_day_cell_ref->get_workout_size(current_lesson); workout_num++)
                {
                    Student* current_workout_student = current_day_cell_ref->get_workout_student(current_lesson, workout_num);
                    bool is_in_vector = false;
                    for (int current_student_repeat_check_id = 0; current_student_repeat_check_id < working_out_students.size(); current_student_repeat_check_id++)
                    {
                        if (working_out_students[current_student_repeat_check_id] == current_workout_student) 
                        { 
                            is_in_vector = true;
                            break;
                        }
                    }
                    if (!is_in_vector) working_out_students.push_back(current_workout_student);
                }
                if (current_internal_lesson != 0) ImGui::SameLine(0.0f, 2.0f);
                std::string add_workout_button_name = generate_label("Отр##add_workout_button", {current_merged_lesson, current_day_cell, current_internal_lesson});
                if (edit_mode && !visible_days[current_day_cell].is_future && ImGui::Button(add_workout_button_name.c_str(), ImVec2(SUBCOLUMN_WIDTH_PXLS, 0.0f)))
                {
                    Group& current_group = current_merged_lesson_ref.get_group();
                    std::tm current_lesson_time = { 0, 0, 0,
                            visible_days[current_day_cell].number - MDAY_DIFF, current_month, current_year };
                    int current_name_id = current_lesson_name_ids[current_internal_lesson];
                    popup_add_working_out = new Popup_Add_Working_Out(all_students, all_lessons, all_days, current_group, current_time, current_lesson_time, current_lesson, &current_merged_lesson_ref);//, current_name_id);
                }
            }
        }
        for (int current_workout_student_id = 0; current_workout_student_id < working_out_students.size(); current_workout_student_id++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(c_str_int(current_group_size + 1 + current_workout_student_id));
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(working_out_students[current_workout_student_id]->get_name().c_str());
            for (int current_day_cell = 0; current_day_cell < visible_days.size(); current_day_cell++)
            {
                bool first_skipped = false;
                for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson]->get_lessons_size(); current_internal_lesson++)
                {
                    if (current_internal_lesson == 0) first_skipped = true;
                    current_lesson.internal_lesson_id = current_internal_lesson;
                    ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);
                    Workout_Info current_workout_info = visible_days[current_day_cell].day->get_workout_info(current_lesson, PTRREF(working_out_students[current_workout_student_id]));
                    if (current_workout_info.internal_lesson == -1) continue;
                    if (current_internal_lesson == 0) first_skipped = false;
                    bool create_fake_radio = current_internal_lesson != 0 && first_skipped;
                    std::string workout_info_radio_tooltip_name = generate_label("##workout_info_radio_tooltip", {current_day_cell, current_workout_student_id, current_internal_lesson, current_lesson.merged_lesson_id}).c_str();
                    if (create_fake_radio)
                    {
                        std::string fake_radio_name = generate_label("##fake_radio", {current_day_cell, 0, current_internal_lesson, current_day_cell}).c_str();
                        ImGui::InvisibleButton(fake_radio_name.c_str(), ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight()));
                    };
                    bool dummy = true;
                    ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
                    ImGui::SameLine();
                    if (ImGui::Checkbox(workout_info_radio_tooltip_name.c_str(), &dummy))
                    {
                        IM_ASSERT(current_workout_info.date.tm_mon == current_time.tm_mon); //not implemented
                        all_days[current_workout_info.date.tm_mday - MDAY_DIFF]->set_status(PTRREF(current_workout_info.lesson_info), current_workout_info.internal_lesson, PTRREF(working_out_students[current_workout_student_id]), STATUS_NO_DATA);
                        visible_days[current_day_cell].day->delete_workout(current_merged_lesson_ref, current_internal_lesson, PTRREF(working_out_students[current_workout_student_id]));
                    }
                    ImGui::SetItemTooltip(to_string(current_workout_info.date, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_begin, current_workout_info.lesson_info->get_lesson_pair(current_workout_info.internal_lesson).time_end).c_str());
                }
            }
        }
        ImGui::EndTable();
        ImGui::EndGroup();
    }
if (disabled) ImGui::EndDisabled();
}
ImGui::PopStyleVar();
ImGui::EndChild();
ImGui::End();
}

