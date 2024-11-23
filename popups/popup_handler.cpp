#include "popup_handler.h"

bool Popup_Handler::is_popup_opened()
{
    return false;
}

void Popup_Handler::open_popup(Popup *popup)
{
    if (popup == nullptr) printf("Warning: using open_popup with nullptr!\n");
    current = popup;
}

bool Popup_Handler::render_popup(Render *render)
{
    if (current)
    {
        bool is_done = current->show_frame();
        if (is_done && current->check_ok())
        {
            current->accept_changes();
        }
        if (is_done)
        {
            delete current;
            current = nullptr;
        }
        return true;
    }
    return false;
}
