#include "subwindow_handler.h"

bool Subwindow_Handler::is_subwindow_opened()
{
    return current != nullptr;
}

bool Subwindow_Handler::can_open_subwindow()
{
    if (is_subwindow_opened())
    {
        return current->allow_ontop();
    }
    else
    {
        return true;
    }
}

void Subwindow_Handler::open_subwindow(Subwindow *subwindow)
{
    if (subwindow == nullptr) return;
    if (is_subwindow_opened())
    {
        if (!current->allow_ontop())
        {
            delete subwindow;
            return;
        }
        delete current;
    }
    current = subwindow;
}

bool Subwindow_Handler::render_subwindow(Render* render)
{
    if (!is_subwindow_opened()) return false;
    if (current->show_frame())
    {
        delete current;
        current = nullptr;
        return true;
    }
    return true;
}