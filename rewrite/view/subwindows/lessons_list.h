#pragma once
#include "subwindow_handler.h"
#include "testwindow.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Lessons_List : public Subwindow
    {
    public:
        static ImVec4 get_background_color()
        {
            return ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
        }
    private:
        bool render_logic() override
        {
            UI::Scope_Color color(ImGuiCol_ChildBg, get_background_color());
            if (ImGui::Button("Вернуться к журналу")) return true;
            ImGui::SameLine();
            // TODO: everything here basically
            if (ImGui::Button("Добавить группу"))
            {
                popup_handler().open_popup(std::make_unique<Add_Or_Edit_Merged_Lesson>(shared(), controller(), shared().wday));
            }
            UI::label("Список всех групп");
            const static size_t COLUMN_COUNT = 6;
            UI::Scope_Child child("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            UI::table("##Список групп", COLUMN_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg, {}, {}, [&]() {
                ImGui::TableSetupColumn("Номер группы");
                ImGui::TableSetupColumn("День недели");
                ImGui::TableSetupColumn("Время посещения");
                ImGui::TableSetupColumn("Описание группы");
                ImGui::TableSetupColumn("Возраст");
                ImGui::TableSetupColumn("Действия");
                ImGui::TableHeadersRow();
                Wday wday = Wday::make_begin_RU();
                do
                {
                    for (auto merged_lesson_it = model()->get_merged_lessons_sorted(wday); !!merged_lesson_it; ++merged_lesson_it)
                    {
                        const auto& merged_lesson = *merged_lesson_it;
                        if (merged_lesson.is_removed(shared().month) && !shared().edit_mode) continue;
                        {
                            UI::Scope_Disabled disabled(merged_lesson.is_removed(shared().month));
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                                UI::label(std::to_string(merged_lesson.get_number()));
                            ImGui::TableSetColumnIndex(1);
                                UI::label(wday.get_name());
                            ImGui::TableSetColumnIndex(2);
                                for (auto it = merged_lesson.cref_internal_lessons().cbegin(); !!it; ++it)
                                {
                                    UI::label(std::to_string(it.get_position().get() + 1) + ". " + Lesson_Infos::get_name(it->get_lesson_type()));
                                    UI::label(it->get_time_begin().to_string() + " - " + it->get_time_end().to_string());
                                }
                            ImGui::TableSetColumnIndex(3);
                                UI::label(merged_lesson.get_comment());
                            ImGui::TableSetColumnIndex(4);
                                UI::label(merged_lesson.get_age_group_string());
                            ImGui::TableSetColumnIndex(5);
                        }

                        UI::Scope_Disabled disabled(shared().month != Month::make_current());
                        int merged_lesson_int = int(merged_lesson_it.get_position().get());
                        std::string restore_label = UI::id("Восстановить группу##", {wday.get_RU(), merged_lesson_int});
                        std::string delete_label = UI::id("Удалить группу##", {wday.get_RU(), merged_lesson_int});
                        std::string edit_label = UI::id("Изменить группу##", {wday.get_RU(), merged_lesson_int});
                        Merged_Lesson_ID merged_lesson_id(wday, merged_lesson_it.get_position());

                        if (shared().edit_mode)
                        {
                            if (merged_lesson.is_removed(shared().month))
                            {
                                if (UI::button_colored(restore_label, UI::RED))
                                {
                                    controller().add(std::make_unique<Restore_Lesson>(merged_lesson_id));
                                }
                            }
                            else
                            {
                                if (UI::button_dangerous(delete_label))
                                {
                                    popup_handler().open_popup(std::make_unique<Confirm_Delete_Lesson>(shared(), controller(), merged_lesson_id));
                                }
                            }
                        }
                        else if (UI::button_dangerous(delete_label))
                        {
                            popup_handler().open_popup(std::make_unique<Confirm_Delete_Lesson>(shared(), controller(), merged_lesson_id));
                        }
                        if (!merged_lesson.is_removed(shared().month) && ImGui::Button(edit_label.c_str()))
                        {
                            popup_handler().open_popup(std::make_unique<Add_Or_Edit_Merged_Lesson>(shared(), controller(), merged_lesson_id));
                        }
                    }
                } while (wday.next());
            });
            return false;
        }
    public:
        Lessons_List(IController& controller, Shared& shared)
        : Subwindow("Список всех групп", controller, shared)
        { }
    };
}