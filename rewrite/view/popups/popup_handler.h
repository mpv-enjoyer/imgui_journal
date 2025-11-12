#pragma once
#include "popup.h"

namespace View
{
    class Popup_Handler
    {
        std::unique_ptr<Popup> current;
    public:
        Popup_Handler() { };
        bool is_popup_opened();
        void open_popup(std::unique_ptr<Popup> popup);
        bool render_popup();
    };
}