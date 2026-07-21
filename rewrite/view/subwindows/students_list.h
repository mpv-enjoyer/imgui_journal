#pragma once
#include "subwindow_handler.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Students_List : public Subwindow
    {
        static ImVec4 get_background_color()
        {
            return ImVec4(0.7f, 0.85f, 0.85f, 1.0f);
        }
        static ImVec4 get_background_color_filtered()
        {
            return ImVec4(0.5f, 0.75f, 0.65f, 1.0f);
        }
        ImGuiTextFilter m_text_filter;
        Vector_Sortable_CIterator_Sorted<Student> m_students_iterator;
        const size_t m_students_count_before;
    public:
        virtual bool render_logic() override
        {
            bool filter_active = std::string(m_text_filter.InputBuf).length() > 0;
            UI::Scope_Color color(ImGuiCol_ChildBg, filter_active ? get_background_color_filtered() : get_background_color());
            if (ImGui::Button("Вернуться к журналу")) return false;
            if (Add_Student_To_Base::IS_FITTING(Time_State::make(shared().month)))
            {
                ImGui::SameLine();
                if (ImGui::Button("Добавить ученика##в общий список"))
                {
                    // TODO: add student to base popup
                    // popup_handler().open_popup(new Popup_Add_Student_To_Base(graphical));
                }
            }
            {
                UI::Scope_Color_Input color;
                m_text_filter.Draw("Поиск с учётом регистра", 140);
            }
            if (filter_active)
            {
                ImGui::SameLine();
                if (UI::button_dangerous("Очистить поиск"))
                {
                    m_text_filter.Clear();
                }
            }
            
            UI::label("Список всех учеников");
            if (!(Time_State::make(shared().month) & Time_State::CurrentYear))
            {
                UI::label("Вы смотрите данные другого года (" + shared().month.get_name() + "). Редактирование доступно только для текущего месяца.", UI::RED);
            }

//     if (should_update_students)
//     {
//         update_lessons_per_student();
//         should_update_students = false;
//     }

//     if (lessons_per_student.size() != journal->student_count())
//         append_students_to_begin();

            UI::Scope_Child child("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            UI::table("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg, {}, {}, [&]() {
                ImGui::TableSetupColumn("Номер д-ра", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Фамилия и имя");
                ImGui::TableSetupColumn("Группы");
                ImGui::TableSetupColumn("Действия", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();

                for (size_t i = model()->students()->cref_students().size(); i-- > m_students_count_before; )
                {

                }
                for (auto it = m_students_iterator; !!it; ++it)
                {
                    
                }
            });

            auto do_student_row = [&](Position<Student> student_pos) -> void
            {
                const Student& student = model()->students()->cref_students()[student_pos];
                int contract_number = model()->students()->cref_contracts()[student.get_contract_pos()].get_number();
                const std::string search_description = std::to_string(contract_number) + " " + student.get_name();
                if (!m_text_filter.PassFilter(search_description.c_str())) return;
                if (shared().edit_mode && student.is_removed(shared().month)) return;
                UI::Scope_ID scope_id(student_pos.get());
                
                {
                    UI::Scope_Disabled disabled(!Edit_Student_In_Base::IS_FITTING(Time_State::make(shared().month)));
                    ImGui::TableNextColumn();
                    UI::Input_Int("##д-р", contract_number, [&](int value) -> bool {
                        controller().add(Ptr<Edit_Student_In_Base>::make(shared().month, student_pos, student.get_name(), value));
                        return true;
                    }, 0);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(-1);
                    UI::Input_Text("##фи", student.get_name(), [&](std::string value) -> bool {
                        controller().add(Ptr<Edit_Student_In_Base>::make(shared().month, student_pos, value, contract_number));
                        return true;
                    });
                }

                ImGui::TableNextColumn();
                auto merged_lessons = model()->get_merged_lessons_for_student(student_pos);
                for (const auto [merged_lesson_id, attendance_student_pos] : merged_lessons)
                {
                    UI::Scope_ID(merged_lesson_id.wday().get_EN());
                    UI::Scope_ID(merged_lesson_id.pos().get());
                    const Attendance_Merged_Lesson& merged_lesson = model()->cref_merged_lesson(merged_lesson_id);
                    if (merged_lesson.is_removed(shared().month)) continue;
                    UI::Scope_Disabled disabled(merged_lesson.is_student_removed(attendance_student_pos, shared().month));
                    UI::Scope_Disabled disabled2(!(Time_State::make(shared().month) & Time_State::CurrentMonth)); // TODO: handle different months...
                    
                    {
                        UI::Scope_Group group;
                        UI::label(merged_lesson_id.wday().get_name_short() + ", " + merged_lesson.get_group_description());
                        if (merged_lesson.cref_internal_lessons().size() > 1)
                        {
                            UI::select_wants_lesson("##attend", merged_lesson, attendance_student_pos, [](std::vector<bool>)... // TODO THIS
                            UI::Select_Lesson_Type select_lesson_type("##attend", [&](std::vector<Lesson_Type> lesson_types) -> bool {

                                return true;
                            })
                        }
                    }
                }
            };
//                 std::string label = generate_label("##attend", {student_id, i});
//                 Attend_Data cached_data = current_group.get_attend_data(internal_student_id);
//                 std::string first_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(0).lesson_name_id);
//                 if (current_lesson_info->get_lessons_size() == 2)
//                 {
//                     std::string second_name = journal->Lesson_name(current_lesson_info->get_lesson_pair(1).lesson_name_id);
//                     if (Graphical::attend_data(label.c_str(), &cached_data, first_name, second_name))
//                     {
//                         journal->set_student_attend_data(current_wday, current_merged_lesson_id, internal_student_id, cached_data);
//                     }
//                 }
//                 else
//                 {
//                     ImGui::Text(first_name.c_str());
//                 }
//                 ImGui::AlignTextToFramePadding();

//                 if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::EndDisabled();
//                 ImGui::SameLine();
//                 if (!current_group.is_deleted(PTRREF(journal->student(student_id))))
//                 {
//                     std::string label_delete = generate_label("Удалить из группы##", {student_id, i});
//                     if (Graphical::button_dangerous(label_delete.c_str()))
//                     {
//                         journal->remove_student_from_group(current_wday, current_merged_lesson_id, student_id);
//                     }
//                 }
//                 else
//                 {
//                     std::string label_restore = generate_label("Восстановить в группе##", {student_id, i});
//                     if (Graphical::button_colored(label_restore.c_str(), 0.1, 0.9, 0.1))
//                     {
//                         journal->restore_student_to_group(current_wday, current_merged_lesson_id, student_id);
//                     }
//                 }

//                 if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::BeginDisabled();

//                 ImGui::SameLine();
//                 std::string button_label = generate_label("Переместить##move", {student_id, i});
//                 if (ImGui::Button(button_label.c_str()))
//                 {
//                     popup_handler->open_popup(new Popup_Move_Student_To_Group(graphical, current_lesson_info, current_wday, current_merged_lesson_id, student_id, &should_update_students));
//                 }
                
//                 ImGui::EndGroup();
//                 if (current_group.is_deleted(PTRREF(journal->student(student_id)))) ImGui::EndDisabled();
//                 if (i != lessons_per_student[index].first.size() - 1)
//                     ImGui::Separator();
//             }
//             if (current_student->is_removed()) ImGui::EndDisabled();
//             ImGui::TableNextColumn();
//             if (!edit_mode)
//             {
//                 if (Graphical::button_dangerous("Удалить ученика"))
//                 {
//                     popup_handler->open_popup(new Popup_Confirm_Delete_Student(graphical, student_id));
//                 }
//             }
//             else
//             {
//                 if (is_removed_input_buffer && Graphical::button_colored("Восстановить ученика", 0.1, 0.9, 0.1))
//                 {
//                     journal->restore_student(student_id);
//                 }
//                 else if (!is_removed_input_buffer && Graphical::button_dangerous("Удалить ученика"))
//                 {
//                     popup_handler->open_popup(new Popup_Confirm_Delete_Student(graphical, student_id));
//                 }
//             }
//             if (!is_current) ImGui::EndDisabled();
//             ImGui::PopID();
//         }
//         ImGui::EndTable();
//     }
//     ImGui::EndChild();
//     ImGui::End();
//     ImGui::PopStyleColor(); //BG
            return true;
        }
        Students_List(IController& controller, Shared& shared)
        : Subwindow("Список всех учеников", controller, shared),
        m_students_iterator(model()->students()->cref_students().csorted_begin()),
        m_students_count_before(model()->students()->cref_students().size())
        {
            
        }
    };
}