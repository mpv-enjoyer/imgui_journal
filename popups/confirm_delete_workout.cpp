#include "confirm_delete_workout.h"

Popup_Confirm_Delete_Workout::Popup_Confirm_Delete_Workout(Graphical *_graphical, const Workout_Info_ *workout_info, const Student* student)
: _workout_info(workout_info)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    Lesson_Pair should_pair;
    Group should_group;
    if (workout_info->real_attend.tm_mon != workout_info->should_attend.tm_mon)
    {
        // should_attend is distant, try to access it to modify
        int should_month = workout_info->should_attend.tm_mon;
        int should_year = journal->workout_handler()->get_year(should_month);
        // do not check for year because only one year is loaded.
        Journal* main_journal = journal->journal_main != nullptr ? journal->journal_main : journal;
        if (main_journal->current_month() == should_month)
        {
            should_pair = main_journal->lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_lesson_pair(workout_info->should_lesson.internal_lesson_id);
            should_group = (main_journal->lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_group());
        }
        else
        {
            // distant attend is not loaded, try to load
            {
                Journal journal_distant(should_month, should_year, main_journal);
                should_pair = journal_distant.lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_lesson_pair(workout_info->should_lesson.internal_lesson_id);
                should_group = (journal_distant.lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_group());
            }
        }
    }
    else
    {
        should_pair = journal->lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_lesson_pair(workout_info->should_lesson.internal_lesson_id);
        should_group = (journal->lesson_info(workout_info->should_attend.tm_wday, workout_info->should_lesson.merged_lesson_id)->get_group());
    }
    for (int i = 0; i < should_group.get_size(); i++)
    {
        if (should_group.get_student(i) == *student)
        {
            _internal_student_id = i;
            break;
        }
    }
    IM_ASSERT(_internal_student_id != -1);
    _should_time_begin = should_pair.time_begin;
    _should_time_end = should_pair.time_end;
    _should_lesson_type = should_pair.lesson_name_id;
    _should_group_description = should_group.get_description();
}

bool Popup_Confirm_Delete_Workout::show_frame()
{
    const Lesson_Info& real_lesson_info = PTRREF(journal->lesson_info(graphical->wday, _workout_info->real_lesson.merged_lesson_id));
    const Group& real_group = real_lesson_info.get_group();
    const Lesson_Pair real_pair = real_lesson_info.get_lesson_pair(_workout_info->real_lesson.internal_lesson_id);
    std::string real_lesson_type = journal->Lesson_name(real_pair.lesson_name_id);
    JTime real_lesson_begin = real_pair.time_begin;
    JTime real_lesson_end = real_pair.time_end;
    std::string real_group_description = real_group.get_description();
    if (begin_frame("Удалить отработку?"))
    {
        ImVec4 red = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
        ImGui::TextColored(red, "Ученик должен был прийти на урок:");
        ImGui::Text(journal->Lesson_name(_should_lesson_type).c_str());
        ImGui::SameLine();
        ImGui::Text(to_string(_should_time_begin, _should_time_end).c_str());
        ImGui::SameLine();
        std::string should_date = std::to_string(_workout_info->should_attend.tm_mday + MDAY_DIFF) + " " + journal->Month_name(_workout_info->should_attend.tm_mon);
        ImGui::Text(should_date.c_str());
        ImGui::Text(("Группа " + _should_group_description).c_str());
        ImGui::TextColored(red, "Ученик отработал на уроке:");
        ImGui::Text(real_lesson_type.c_str());
        ImGui::SameLine();
        ImGui::Text(to_string(real_lesson_begin, real_lesson_end).c_str());
        ImGui::SameLine();
        std::string real_date = std::to_string(_workout_info->real_attend.tm_mday + MDAY_DIFF) + " " + journal->Month_name(_workout_info->real_attend.tm_mon);
        ImGui::Text(real_date.c_str());
        ImGui::Text(("Группа " + real_group_description).c_str());
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Confirm_Delete_Workout::accept_changes()
{
    IM_ASSERT(check_ok());
    const Workout_Info_* workout_info = _workout_info;
    Lesson should_lesson = workout_info->should_lesson;
    int internal_student_id = _internal_student_id;
    Student_Status new_status;
    new_status.discount_status = -1;
    new_status.status = STATUS_NO_DATA;
    if (workout_info->real_attend.tm_mon != workout_info->should_attend.tm_mon)
    {
        // should_attend is distant, try to access it to modify
        int should_month = workout_info->should_attend.tm_mon;
        int should_year = journal->workout_handler()->get_year(should_month);
        // do not check for year because only one year is loaded.
        Journal* main_journal = journal->journal_main != nullptr ? journal->journal_main : journal;
        if (main_journal->current_month() == should_month)
        {
            main_journal->set_lesson_status(workout_info->should_attend.tm_mday, should_lesson, internal_student_id, new_status, true);
        }
        else
        {
            // distant attend is not loaded, try to load
            {
                Journal journal_distant(should_month, should_year, main_journal);
                journal_distant.set_lesson_status(workout_info->should_attend.tm_mday, should_lesson, internal_student_id, new_status, true);
            }
            // journal destructor will save the changes or discard saving them
            journal->load_workouts();
        }
    }
    else
    {
        journal->set_lesson_status(workout_info->should_attend.tm_mday, should_lesson, internal_student_id, new_status, true);
    }
}