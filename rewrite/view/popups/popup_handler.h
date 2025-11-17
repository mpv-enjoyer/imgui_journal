#pragma once
#include "popup.h"

namespace View
{
    class Popup_Handler
    {
        std::unique_ptr<Popup> current;
    public:
        Popup_Handler() { };
        bool is_popup_opened()
        {
            return !!current;
        }
        void open_popup(std::unique_ptr<Popup> popup)
        {
            if (!current) current = std::move(popup);
        }
        bool render_popup()
        {
            if (!current) return false;
            if (current->render()) current.reset();
            return true;
        }
    };
}
