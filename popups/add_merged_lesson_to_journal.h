#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Add_Merged_Lesson_To_Journal : public Popup
{
private:
    int day_of_the_week;
    int group_number = 0;
    std::string group_comment;
    int combo_lesson_name_id = 0;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Popup_Add_Merged_Lesson_To_Journal(); 
    bool show_frame();
    bool is_ok_possible();
    void accept_changes();
};