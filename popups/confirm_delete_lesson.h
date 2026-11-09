#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"
#include "../graphical/graphical.h"

class Popup_Confirm_Delete_Lesson : public Popup
{
    int _merged_lesson_id;
    int _wday;
public:
    Popup_Confirm_Delete_Lesson(JournalHolder* _graphical, int wday, int merged_lesson_id);
    bool show_frame() override;
    bool is_ok_possible() override { return true; }
    void accept_changes() override;
};