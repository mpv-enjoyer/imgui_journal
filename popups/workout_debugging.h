#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

// Some workouts are unsynced already, this debug tool will help me.

class Popup_Workout_Debugging : public Popup
{
    Student_Status get_should_student_status(const Workout_Info_* workout);
public:
    Popup_Workout_Debugging(JournalHolder* graphical);
    bool show_frame() override;
    bool is_ok_possible() override { return true; }
    void accept_changes() override;
};