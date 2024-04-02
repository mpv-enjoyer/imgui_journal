#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Add_Student_To_Base : public Popup
{
private:
    std::string name = "";
    bool is_date_visible = false;
    int contract = 0;
    int age_group = 0;
public:
    Popup_Add_Student_To_Base() {};
    bool show_frame();
    bool is_ok_possible() 
    {
        if (contract < 0) error("SYSTEM_NEGATIVE_CONTRACT");
        return contract >= 0;
    }
    void accept_changes();
};