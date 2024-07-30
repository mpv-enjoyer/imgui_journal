#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Edit_Lesson : public Popup
{
    const Lesson_Info* lesson_info;
    Lesson_Pair first_lesson_pair;
    Lesson_Pair second_lesson_pair;
    bool second_lesson_exists = false;
    std::string group_description;
    int group_number;
    int wday;
    int merged_lesson_id;
public:
    Popup_Edit_Lesson(Graphical* graphical, int wday, int merged_lesson_id);
    bool show_frame();
    void accept_changes();
    bool is_ok_possible()
    {
        if (second_lesson_exists)
        {
            if (INTERVAL_CHECK(first_lesson_pair.time_begin, first_lesson_pair.time_end, second_lesson_pair.time_begin, second_lesson_pair.time_end))
            {
                error("неверное время"); return false;
            }
        }
        return true;
    };
};