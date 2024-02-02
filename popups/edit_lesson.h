#pragma once
#include "popups.h"

class Popup_Edit_Lesson : public Popup
{
    Lesson_Info& lesson_info;
    Lesson_Pair first_lesson_pair;
    Lesson_Pair second_lesson_pair;
    bool second_lesson_exists = false;
    std::string group_description;
    int group_number;
public:
    Popup_Edit_Lesson(Lesson_Info& lesson_info);
    bool show_frame();
    bool is_ok_possible()
    {
        if (second_lesson_exists)
        {
            if (SWAPOVERLAP(first_lesson_pair.time_begin, first_lesson_pair.time_end, second_lesson_pair.time_begin, second_lesson_pair.time_end))
            {
                error("неверное время"); return false;
            }
        }
        return true;
    };
    void accept_changes();
};