#include "workout_debugging.h"

Student_Status Popup_Workout_Debugging::get_should_student_status(const Workout_Info_* workout)
{
    const Student& student = PTRREF(journal->student(workout->should_student_id));
    const Lesson_Info& lesson_info = PTRREF(journal->lesson_info(workout->should_attend.tm_wday, workout->should_lesson.merged_lesson_id));
    return journal->day(workout->should_attend.tm_mday)->get_status(lesson_info, workout->should_lesson.internal_lesson_id, student);
}

Popup_Workout_Debugging::Popup_Workout_Debugging(JournalHolder *graphical)
    : Popup(graphical) {}

bool Popup_Workout_Debugging::show_frame()
{
    if (begin_frame("Добавить ученика в базу"))
    {
        const std::vector<const Workout_Info_*> all_workouts_should_this_month = journal->workout_handler()->search_info(journal->current_month());
        if (ImGui::BeginTable("Отработки за текущий месяц", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg))
        {
            for (size_t i = 0; i < all_workouts_should_this_month.size(); i++)
            {
                const auto& workout = all_workouts_should_this_month[i];
                Student_Status status = get_should_student_status(workout);
                ImGui::TableNextColumn();
                ImGui::Text("%s", std::to_string(i).c_str());
                ImGui::TableNextColumn();
                ImGui::Text("Статус %i (%s)", status.status, status.status == STATUS_WORKED_OUT ? "ОК" : "НЕ ОК");
            }
            ImGui::EndTable();
        }

        ImGui::Text("К удалению: ");
        ImGui::Text(" ");
        for (size_t i = 0; i < all_workouts_should_this_month.size(); i++)
        {
            Student_Status status = get_should_student_status(all_workouts_should_this_month[i]);
            if (status.status != STATUS_WORKED_OUT)
            {
                ImGui::SameLine();
                ImGui::Text(std::to_string(i).c_str());
            }
        }

        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Workout_Debugging::accept_changes()
{
    IM_ASSERT(check_ok());
    bool done = false;
    while (!done)
    {
        done = true;
        const std::vector<const Workout_Info_*> all_workouts_should_this_month = journal->workout_handler()->search_info(journal->current_month());
        for (auto workout : all_workouts_should_this_month)
        {
            Student_Status status = get_should_student_status(workout);
            if (status.status != STATUS_WORKED_OUT)
            {
                journal->workout_handler()->delete_info(workout);
                // all_workouts_should_this_month is now invalidated
                done = false;
                break;
            }
        }
    }
}