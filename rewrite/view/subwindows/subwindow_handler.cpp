#include "subwindow_handler.h"

bool View::Subwindow_Handler::is_subwindow_opened()
{
    return !!current;
}

void View::Subwindow_Handler::open_subwindow(std::unique_ptr<Subwindow> subwindow)
{
    if (!current || current->allow_ontop())
    {
        current = std::move(subwindow);
    }
}

bool View::Subwindow_Handler::render_subwindow()
{
    if (!current) return false;
    if (current->render()) current.reset();
    return true;
}