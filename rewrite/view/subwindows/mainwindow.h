#pragma once
#include "subwindow_handler.h"
#include "testwindow.h"
#include "lessons_list.h"
#include "view/shared.h"
#include "controller/commands/commands.h"
#include "common/sort_ordered.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        static const int SUBCOLUMN_WIDTH_PXLS = 40;
        const std::function<bool(const Student&, const Student&)> M_COMPARE_STUDENTS;
        const std::function<bool(const AStudent&, const AStudent&)> M_COMPARE_ATTENDANCE_STUDENTS;
        struct
        {
            std::vector<std::pair<Attendance_ID, std::string>> workouts_should;
            bool want_update = true;
        } m_buf;
        void update_buf()
        {
            for (auto merged_lesson_it = model()->cref_wday(shared().wday).cref_merged_lessons().csorted_begin(); !!merged_lesson_it; ++merged_lesson_it)
            {
                Merged_Lesson_ID merged_lesson_id(shared().wday, merged_lesson_it.get_position());
                for (auto internal_lesson_it = model()->cref_merged_lesson(merged_lesson_id).cref_internal_lessons().cbegin(); !!internal_lesson_it; ++internal_lesson_it)
                {
                    Internal_Lesson_ID internal_lesson_id(merged_lesson_id, internal_lesson_it.get_position());
                    for (auto internal_student_it = model()->cref_internal_lesson(internal_lesson_id).cref_students().csorted_begin(M_COMPARE_ATTENDANCE_STUDENTS); !!internal_student_it; ++internal_student_it)
                    {
                        Internal_Student_ID internal_student_id(internal_lesson_id, internal_student_it.get_position());
                        for (int aday = 0; aday < shared().wday.calculate_count_for_bottom_year(model()->bottom_year); aday++)
                        {
                            Attendance_ID attendance_id(internal_student_id, Aday::make_from_index(aday));
                            if (auto workout_id = model()->workouts()->is_should(attendance_id))
                            {
            //auto workout_info = journal->workout_handler()->get_info(journal->current_month(), visible_day.number - MDAY_DIFF, lesson, found_student_id);
            //tooltip = "Отработан " + std::to_string(workout_info->real_attend.tm_mday + MDAY_DIFF) + " " + journal->Month_name(workout_info->real_attend.tm_mon) + ", " + journal->Wday_name(workout_info->real_attend.tm_wday);

                                Wday real_wday = workout_id->real_internal_lesson_id().wday();
                                Aday real_aday = workout_id->real_aday();
                                Mday real_mday = Mday::make_from_aday(model()->bottom_year, real_wday, real_aday);
                                std::string workout_desc = "Отработан " + std::to_string(real_mday.get_from_1()) + " " + real_mday.get_month().get_name() + ", " + real_wday.get_name();
                                m_buf.workouts_should.emplace_back(attendance_id, workout_desc);
                            }
                        }
                    }
                }
            }
            m_buf.want_update = false;
        }

        

        // const Attendance_Merged_Lesson& merged_lesson(Position<Attendance_Merged_Lesson> pos)
        // {
        //     return model()->attendance_wdays()->cref_wday(shared().wday).cref_merged_lessons()[pos];
        // }

        void attendance_combo(std::string label, Attendance_ID id, std::size_t& workouts_buf_offset)
        {
            ImGui::SetNextItemWidth(SUBCOLUMN_WIDTH_PXLS);
            AStatus status = model()->cref_attendance_holder(id).get_status();
            if (status == AStatus::NOT_AWAITED)
            {
                ImVec2 gradient_size = ImVec2(SUBCOLUMN_WIDTH_PXLS, ImGui::GetFrameHeight());
                {
                    ImVec2 p0 = ImGui::GetCursorScreenPos();
                    ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    draw_list->AddRectFilled(p0, p1, IM_COL32(135, 135, 135, 255));
                    ImGui::InvisibleButton("##gradient1", gradient_size);
                }
                return;
            }

            const std::vector<const char*> items = { " ", "V", "Б", "O", "ОТР" };
            int status_id = static_cast<std::underlying_type_t<AStatus>>(status);
            const char* combo_preview_value = items[status_id];  // Pass in the preview value visible before opening the combo (it could be anything)
            std::string tooltip = "";
            bool modify_for_workout = false;
            if (workouts_buf_offset != m_buf.workouts_should.size() && m_buf.workouts_should[workouts_buf_offset].first == id)
            {
                tooltip = m_buf.workouts_should[workouts_buf_offset].second;
                workouts_buf_offset++;
                modify_for_workout = true;
            }
            ImGuiComboFlags flags = modify_for_workout ? ImGuiComboFlags_NoArrowButton : 0; 
            if (ImGui::BeginCombo(label.c_str(), combo_preview_value, flags))
            {
                for (int n = 0; n < items.size() - (!modify_for_workout); n++)
                {
                    const bool is_selected = (status_id == n);
                    if (ImGui::Selectable(items[n], is_selected))
                    {
                        if (status_id != n)
                        {
                            // n changed:
                            controller().add(std::make_shared<Set_Lesson_Status>(id, static_cast<AStatus>(n)));
                        }
                        ImGui::EndCombo();
                        return;
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (modify_for_workout) ImGui::SetItemTooltip(tooltip.c_str());
        }

        bool table_row(Merged_Lesson_ID merged_lesson_id, Pos<Student> student_pos, Pos<AStudent> attendance_student_pos, int counter, std::vector<AMerged_Lesson::Aday_With_Status> adays)
        {
            const AMerged_Lesson& merged_lesson = model()->cref_merged_lesson(merged_lesson_id);
            const Student& student = model()->students()->cref_students()[student_pos];
            const bool skip = [&](){
                for (auto aday : adays)
                {
                    if (!aday.is_active) continue;
                    for (const auto& internal_lesson : merged_lesson.cref_internal_lessons())
                    {
                        if (internal_lesson.cref_student(attendance_student_pos).cref_holder(aday.aday).get_status() == AStatus::NOT_AWAITED)
                        {
                            continue;
                        }
                        return false;
                    }
                }
                return true; // Calculate visibility without using Removal_Info. That's for later use.
            }();
            if (skip) return false;
            // TODO_CRITICAL("disabled if: student is deleted/student is deleted from a group/student is deleted from a internal lesson/student went to a single lesson(UNIMPL)");
            // const int contract = model()->students()->cref_contracts()[model()->students()->cref_students()[student_pos].get_contract_pos()].get_number();
            //if (disabled && !graphical->edit_mode && !group.is_moved_away(student)) return false;
            // ImGui::TableNextRow();
            // if (disabled)
            // {
                // ImGui::BeginDisabled();
                // ImGui::TableSetColumnIndex(0); ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),"-");
            // }
            // else
            // {
                // ImGui::TableSetColumnIndex(0); ImGui::Text("%i", counter);
            // }
            // ImGui::TableSetColumnIndex(1); ImGui::Text(student.get_name().c_str());
            // ImGui::TableSetColumnIndex(2); ImGui::Text("%i", student.get_contract());
            // Attend_Data attend_data = group.get_attend_data(internal_student_id);
            // std::vector<bool> is_internal_lesson_enabled;
            // if (merged_lesson.get_lessons_size() == 1)
                // is_internal_lesson_enabled.push_back(true);
            // else
            // {
                // if (attend_data == ATTEND_FIRST) is_internal_lesson_enabled = {true, false};
                // if (attend_data == ATTEND_SECOND) is_internal_lesson_enabled = {false, true};
                // if (attend_data == ATTEND_BOTH) is_internal_lesson_enabled = {true, true};
            // }
            // int show_price_sum = 0;
            // std::string show_lesson_name = journal->merged_lesson_name(graphical->wday, merged_lesson_id, internal_student_id);
            // for (int i = 0; i < merged_lesson.get_lessons_size(); i++)
            // {
                // if (!is_internal_lesson_enabled[i]) continue;
                // int lesson_type = merged_lesson.get_lesson_pair(i).lesson_name_id;
                // show_price_sum += journal->lesson_common_price(contract, lesson_type);
            // }
            // ImGui::TableSetColumnIndex(3);
            // ImGui::Text(show_lesson_name.c_str());
            // ImGui::TableSetColumnIndex(4); ImGui::Text(c_str_int(show_price_sum));
// 
            // int price_sum = 0;
            // for (int current_day_cell = 0; current_day_cell < graphical->visible_days.size(); current_day_cell++)
            // {
                // price_sum += table_cell(merged_lesson_id, internal_student_id, current_day_cell);
            // }
            // ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + graphical->visible_days.size());
            // ImGui::TextDisabled(c_str_int(price_sum));
            // if (disabled) ImGui::EndDisabled();
            // return !disabled;
            return true;
        }

        void table(Merged_Lesson_ID merged_lesson_id)
        {
            const AMerged_Lesson& merged_lesson = model()->cref_merged_lesson(merged_lesson_id);
            if (!shared().edit_mode && merged_lesson.is_removed()) return;
            UI::Scope_Disabled disabled(merged_lesson.is_removed());
            UI::Scope_Group group;
            if (disabled)
            {
                UI::label("Удаленная", UI::RED); ImGui::SameLine();
            }
            UI::label(merged_lesson.get_description());
            std::string table_name = "##table" + std::to_string(merged_lesson_id.pos().get());
            static const int DEFAULT_COLUMN_COUNT = 6;
            auto all_adays = merged_lesson.get_adays();
            size_t begin_aday_index = Aday::make_from_first_wday(shared().wday, shared().month).index();
            size_t end_aday_index = begin_aday_index + shared().month.calculate_wday_count(shared().wday);
            std::vector<Aday_With_Status> adays(all_adays.begin() + begin_aday_index, all_adays.begin() + end_aday_index);
            int table_columns = DEFAULT_COLUMN_COUNT + adays.size();
            UI::table(table_name, table_columns, 
            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoPadInnerX,
            ImVec2(std::numeric_limits<float>::max(),(0.0F)), {}, [&]() {
                ImU32 row_bg_color = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
                ImGui::TableNextColumn(); UI::label("No");
                ImGui::TableNextColumn(); UI::label("ФИ ученика");
                ImGui::TableNextColumn(); UI::label("Дог-р");
                ImGui::TableNextColumn(); UI::label("Программа");
                ImGui::TableNextColumn(); UI::label("Цена");
                for (auto aday : adays)
                {
                    ImGui::TableNextColumn();
                    Mday mday = Mday::make_from_aday(shared().month.get_study_bottom_year(), shared().wday, aday.aday);
                    if (!aday.is_active) UI::label("--");
                    else UI::label(mday.to_string());
                }
                ImGui::TableNextColumn();
                ImGui::Text("Сумма");
            });
            auto student_positions = merged_lesson.get_student_positions();
            auto compare_students_by_pos_id = [&](std::size_t l, std::size_t r){
                return M_COMPARE_STUDENTS(model()->students()->cref_students()[student_positions[l]], 
                                          model()->students()->cref_students()[student_positions[r]]);
            };
            int counter = 1;
            for (std::size_t i : sort_ordered(student_positions, compare_students_by_pos_id))
            {
                bool enabled = table_row(merged_lesson_id, student_positions[i], Pos<AStudent>(i), counter, adays);
                if (enabled) counter++;
            }
        }

        bool render_logic() override
        {
            if (UI::button_colored("Группы", Lessons_List::get_background_color()))
            {
                subwindow_handler().open_subwindow(std::make_unique<Lessons_List>(controller(), shared()));
            }
            auto& merged_lessons = model()->attendance_wdays()->cref_wday(shared().wday).cref_merged_lessons();
            const AMerged_Lesson* previous = nullptr;
            for (auto iter = merged_lessons.csorted_begin(); iter; ++iter )
            {
                auto iter_begin_time = iter->cref_internal_lessons().cbegin()->get_time_begin();
                bool need_sameline = false;
                if (previous && iter_begin_time == previous->cref_internal_lessons().cbegin()->get_time_begin())
                {
                    need_sameline = true;
                }
                if (!shared().edit_mode && previous && previous->is_removed()) need_sameline = false;
                if (need_sameline) ImGui::SameLine();
                Merged_Lesson_ID merged_lesson_id(shared().wday, iter.get_position());
                table(merged_lesson_id);
                previous = &iter.get();
            }
            return ImGui::Button("Exit lol");
        }
    public:
        Mainwindow(IController& controller, Shared& shared)
        : Subwindow("mainwindow", controller, shared),
        M_COMPARE_STUDENTS([&](const Student& l_student, const Student& r_student) -> bool {
            auto l_contract = model()->students()->cref_contracts()[l_student.get_contract_pos()].get_number();
            auto r_contract = model()->students()->cref_contracts()[r_student.get_contract_pos()].get_number();
            auto l_name = l_student.get_name();
            auto r_name = r_student.get_name();
            return std::tie(l_contract, l_name) < std::tie(r_contract, r_name);
        }),
        M_COMPARE_ATTENDANCE_STUDENTS([&](const AStudent& l, const AStudent& r) -> bool {
            auto l_student = model()->students()->cref_students()[l.get_student_pos()];
            auto r_student = model()->students()->cref_students()[r.get_student_pos()];
            return M_COMPARE_STUDENTS(l_student, r_student);
        })
        {
            subwindow_handler().open_subwindow(std::make_unique<Testwindow>(controller, shared));
        }
    };
}