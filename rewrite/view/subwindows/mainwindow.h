#pragma once
#include "subwindow_handler.h"
#include "testwindow.h"
#include "lessons_list.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        // const Attendance_Merged_Lesson& merged_lesson(Position<Attendance_Merged_Lesson> pos)
        // {
        //     return model()->attendance_wdays()->cref_wday(shared().wday).cref_merged_lessons()[pos];
        // }
        void table(Merged_Lesson_ID merged_lesson_id)
        {
            const Attendance_Merged_Lesson& merged_lesson = model()->cref_merged_lesson(merged_lesson_id);
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
                    UI::label(Mday::make_from_aday(shared().month.get_study_bottom_year(), shared().wday, aday.aday).to_string());
                }
                ImGui::TableNextColumn();
                ImGui::Text("Сумма");
            });
        }

        bool render_logic() override
        {
            if (UI::button_colored("Группы", Lessons_List::get_background_color()))
            {
                subwindow_handler().open_subwindow(std::make_unique<Lessons_List>(controller(), shared()));
            }
            auto& merged_lessons = model()->attendance_wdays()->cref_wday(shared().wday).cref_merged_lessons();
            const Attendance_Merged_Lesson* previous = nullptr;
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
        : Subwindow("mainwindow", controller, shared)
        {
            subwindow_handler().open_subwindow(std::make_unique<Testwindow>(controller, shared));
        }
    };
}