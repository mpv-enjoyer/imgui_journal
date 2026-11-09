#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"
#include "../graphical/graphical.h"

class Popup_Confirm_Delete_Student : public Popup
{
    int _student_id;
public:
    Popup_Confirm_Delete_Student(JournalHolder* _graphical, int student_id);
    bool show_frame() override;
    bool is_ok_possible() override { return true; }
    void accept_changes() override;
};