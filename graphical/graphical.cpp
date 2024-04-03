#include "graphical.h"

Graphical::Graphical(Journal& main_journal) : journal(main_journal)
{
    _edit_mode = false;
    _wday = journal.current_time.tm_wday;
    _visible_days = journal.enumerate_days(_wday);
}

void Graphical::select_wday(int wday)
{
    IM_ASSERT(wday >= 0 && wday < 7);
    _wday = wday;
}

void Graphical::set_edit_mode(bool value)
{
    _edit_mode = value;
}

