#pragma once
#include "popup.h"

namespace View
{
    class Popup_Handler
    {
        std::unique_ptr<Popup> current;
    public:
        Popup_Handler() { };
        bool is_subwindow_opened();
        void open_subwindow(std::unique_ptr<Popup> popup);
        bool render_subwindow();
    };
}