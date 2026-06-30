#pragma once
#include "common/modifiers.h"
#include "common/jtime.h"

namespace View
{
    class Subwindow_Handler;
    class Popup_Handler;

    struct Shared
    {
        NON_COPYABLE_NOR_MOVABLE(Shared);
        bool edit_mode = false;
        Month month = Month::make_current();
        Wday wday = Wday::make_current();
        Subwindow_Handler& subwindow_handler;
        Popup_Handler& popup_handler;
        Shared(Subwindow_Handler& subwindow_handler_, Popup_Handler& popup_handler_)
        : subwindow_handler(subwindow_handler_), popup_handler(popup_handler_) { }
        // We want to start the program with current month, wday opened.
        // There *should* be Model Year covering this month.
    };
}