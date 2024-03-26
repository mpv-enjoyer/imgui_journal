#include "internal.h"
#include "render.h"

void Graphical::select_wday(int wday)
{
    IM_ASSERT(wday >= 0 && wday < 7);
    _wday = wday;
}

bool Graphical::is_edit_mode()
{
    return _edit_mode;
}

void Graphical::set_edit_mode(bool value)
{
    _edit_mode = value;
}

const std::vector<Day_With_Info> &Graphical::visible_days()
{
    return _visible_days;
}

const int Graphical::wday()
{
    return _wday;
}