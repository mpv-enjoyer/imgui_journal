#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"
#include "../graphical/graphical.h"

class Popup_Confirm_Delete_Workout : public Popup
{
    JTime _should_time_begin;
    JTime _should_time_end;
    int _should_lesson_type;
    int _internal_student_id = -1;
    Journal* _journal;
    std::string _should_group_description;
    const Workout_Info_* _workout_info;
public:
    Popup_Confirm_Delete_Workout(Graphical* _graphical, const Workout_Info_* workout_info, int student_id);
    bool show_frame();
    bool is_ok_possible() { return true; }
    void accept_changes();
};