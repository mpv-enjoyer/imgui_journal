#include "popup_handler.h"

bool View::Popup_Handler::is_popup_opened()
{
    return !!current;
}

void View::Popup_Handler::open_popup(std::unique_ptr<Popup> popup)
{
    if (!current) current = std::move(popup);
}

bool View::Popup_Handler::render_popup()
{
    if (!current) return false;
    if (current->render()) current.reset();
    return true;
}