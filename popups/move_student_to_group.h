#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Move_Student_To_Group : public Popup
{
    const Lesson_Info* current_lesson_info;
    const int current_lesson_info_wday;
    const int current_lesson_info_id;
    const int student_id;
    bool* callback; // TODO: do something similar in every popup
    int select_wday = -1;
    int select_merged_lesson_id = -1;
public:
    Popup_Move_Student_To_Group(Graphical* graphical, const Lesson_Info* current, int current_lesson_info_wday, int current_lesson_info_id, int student_id, bool* callback);
    bool show_frame();
    void accept_changes();
    bool is_ok_possible()
    {
        if (select_wday == -1)
        {
            error("выберите группу");
            return false;
        }
        if (select_merged_lesson_id == -1) return false;
        return true;
    };
};