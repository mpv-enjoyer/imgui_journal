#pragma once
#include "popups.h"

class Popup_Select_Day_Of_The_Week : public Popup
{
private:
    int day_of_the_week = 0;
    int month = 0;
    int year = 0;
public:
    Popup_Select_Day_Of_The_Week();
    Popup_Select_Day_Of_The_Week(int current_day_of_the_week);
    bool show_frame();
    bool is_ok_possible() { return true; }
    int get_day_of_the_week() { IM_ASSERT(check_ok()); return day_of_the_week; }
    int get_month() { IM_ASSERT(check_ok()); return month; }
    int get_year() { IM_ASSERT(check_ok()); return year - 1900; }
    void accept_changes(int& current_day_of_the_week); //TODO: here I should update some other currents
};