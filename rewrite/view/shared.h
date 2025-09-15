#pragma once
#include "common/modifiers.h"
#include "model/jtime.h"

namespace View
{
    struct Shared
    {
        NON_COPYABLE_NOR_MOVABLE(Shared);
        bool edit_mode = false;
        Month month = Month::make_current();
        Wday wday = Wday::make_current();
        Shared() { }
        // We want to start the program with current month, wday opened.
        // There *should* be Model Year covering this month.
    };
}