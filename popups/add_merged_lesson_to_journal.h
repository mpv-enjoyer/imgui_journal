#pragma once
#include "popups.h"

class Popup_Add_Merged_Lesson_To_Journal : public Popup
{
private:
    int day_of_the_week;
    const std::vector<Group*>& all_groups;
    int group_number = 0;
    std::string group_comment;
    int combo_lesson_name_id = 0;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Popup_Add_Merged_Lesson_To_Journal(const std::vector<Group*>& all_groups, int current_day_of_the_week); 
    bool show_frame();
    bool is_ok_possible();
    void accept_changes(std::vector<Group*>& all_groups, std::vector<Lesson_Info*>& lessons_in_this_day, const std::vector<Visible_Day>& visible_days);
};