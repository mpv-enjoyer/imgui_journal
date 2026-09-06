#pragma once
#include "subwindow_handler.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Students_List : public Subwindow
    {
        static ImVec4 get_background_color_filtered()
        {
            return ImVec4(0.5f, 0.75f, 0.65f, 1.0f);
        }
        ImGuiTextFilter m_text_filter;
        Vector_Sortable_CIterator_Sorted<Student> m_students_iterator;
        const size_t m_students_count_before;
    public:
        static ImVec4 get_background_color()
        {
            return ImVec4(0.7f, 0.85f, 0.85f, 1.0f);
        }
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
            ImGui::SameLine();
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
                    }, 0).render();
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(-1);
                    UI::Input_Text("##фи", student.get_name(), [&](std::string value) -> bool {
                        controller().add(Ptr<Edit_Student_In_Base>::make(shared().month, student_pos, value, contract_number));
                        return true;
                    }).render();
                }

                ImGui::TableNextColumn();
                auto merged_lessons = model()->get_merged_lessons_for_student(student_pos);
                bool print_separator = false;
                for (const auto& [merged_lesson_id, attendance_student_pos] : merged_lessons)
                {
                    if (print_separator)
                    {
                        ImGui::Separator();
                    }
                    else
                    {
                        print_separator = true;
                    }
                    UI::Scope_ID id1(merged_lesson_id.wday().get_EN());
                    UI::Scope_ID id2(merged_lesson_id.pos().get());
                    const Attendance_Merged_Lesson& merged_lesson = model()->cref_merged_lesson(merged_lesson_id);
                    if (merged_lesson.is_removed(shared().month)) continue;
                    UI::Scope_Disabled disabled2(!(Time_State::make(shared().month) & Time_State::CurrentMonth)); // TODO: handle different months...
                    UI::Scope_Group group;
                    {
                        UI::Scope_Disabled disabled(merged_lesson.is_student_removed(attendance_student_pos, shared().month));
                        UI::label(merged_lesson_id.wday().get_name_short() + ", " + merged_lesson.get_group_description());
                        if (merged_lesson.cref_internal_lessons().size() > 1)
                        {
                            UI::select_wants_lesson("##attend", merged_lesson, attendance_student_pos, [&](std::vector<bool> wants) -> bool {
                                Month month = shared().month;
                                do
                                {
                                    for (std::size_t i = 0; i < wants.size(); i++)
                                    {
                                        Internal_Lesson_ID internal_lesson_id(merged_lesson_id, Position<Attendance_Internal_Lesson>(i));
                                        Internal_Student_ID id(internal_lesson_id, attendance_student_pos);
                                        auto call = std::make_shared<Edit_Wants_Lesson>(month, id, wants[i]);
                                        if (!call->get_error(model()))
                                        {
                                            controller().add(call);
                                        }
                                    }
                                } while (month.next());
                                return true;
                            });
                        }
                        else
                        {
                            UI::label(Lesson_Infos::get_name(merged_lesson.cref_first_internal_lesson().get_lesson_type()));
                        }
                        ImGui::AlignTextToFramePadding();
                    }

                    ImGui::SameLine();
                    if (!merged_lesson.is_student_removed(attendance_student_pos, shared().month))
                    {
                        if (UI::button_dangerous("Удалить из группы?"))
                        {
                            controller().add(std::make_shared<Remove_Student_From_Lesson>(merged_lesson_id, attendance_student_pos));
                        }
                    }
                    else
                    {
                        if (UI::button_colored("Восстановить в группе", UI::GREEN))
                        {
                            controller().add(std::make_shared<Restore_Student_To_Lesson>(merged_lesson_id, attendance_student_pos));
                        }
                    }

                    ImGui::SameLine();
                    {
                        UI::Scope_Disabled disabled(merged_lesson.is_student_removed(attendance_student_pos, shared().month));
                        if (ImGui::Button("Переместить"))
                        {
                            /* TODO: MOVE POPUP */
                        }
                    }
                }

                ImGui::TableNextColumn();
                if (!shared().edit_mode)
                {
                    if (UI::button_dangerous("Удалить ученика"))
                    {
                        /* TODO: CONFIRM DELETE STUDENT POPUP */
                    }
                }
                else if (student.is_removed(shared().month))
                {
                    if (UI::button_colored("Восстановить ученика", UI::GREEN))
                    {
                        controller().add(std::make_shared<Restore_Student>(student_pos));
                    }
                }
                else
                {
                    if (UI::button_dangerous("Удалить ученика"))
                    {
                        /* TODO: POPUP CONFIRM DELETE STUDENT */
                    }
                }
            };

            UI::table("students", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg, {}, {}, [&]() {
                ImGui::TableSetupColumn("Номер д-ра", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Фамилия и имя");
                ImGui::TableSetupColumn("Группы");
                ImGui::TableSetupColumn("Действия", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();

                for (size_t i = model()->students()->cref_students().size(); i-- > m_students_count_before; )
                {
                    do_student_row(Position<Student>(i));
                }
                for (auto it = m_students_iterator; !!it; ++it)
                {
                    do_student_row(it.get_position());
                }
            });
            return true;
        }
        Students_List(IController& controller, Shared& shared)
        : Subwindow("Список всех учеников", controller, shared),
        m_students_iterator(model()->get_all_students_sorted()),
        m_students_count_before(model()->students()->cref_students().size())
        {
            
        }
    };
}