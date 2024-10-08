#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Select_Day_Of_The_Week : public Popup
{
private:
    int day_of_the_week = 0;
    int month = 0;
    int year = 0;
public:
    Popup_Select_Day_Of_The_Week(Graphical* graphical);
    bool show_frame();
    void accept_changes();
    bool is_ok_possible() { return true; }
    int get_day_of_the_week() { IM_ASSERT(check_ok()); return day_of_the_week; }
    int get_month() { IM_ASSERT(check_ok()); return month; }
    int get_year() { IM_ASSERT(check_ok()); return year - 1900; }
};