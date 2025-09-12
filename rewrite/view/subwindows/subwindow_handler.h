#pragma once
#include "subwindow.h"

namespace View
{
    class Subwindow_Handler
    {
        std::unique_ptr<Subwindow> current;
    public:
        Subwindow_Handler() { };
        bool is_subwindow_opened();
        void open_subwindow(std::unique_ptr<Subwindow> subwindow);
        bool render_subwindow();
    };
}