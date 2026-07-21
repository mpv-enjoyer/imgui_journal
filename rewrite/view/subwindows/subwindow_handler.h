#pragma once
#include "subwindow.h"

namespace View
{
    class Subwindow_Handler
    {
        std::unique_ptr<Subwindow> current;
    public:
        Subwindow_Handler() { };
        bool is_subwindow_opened()
        {
            return !!current;
        }
        void open_subwindow(std::unique_ptr<Subwindow> subwindow)
        {
            if (!current || current->allow_ontop())
            {
                current = std::move(subwindow);
            }
        }
        bool render_subwindow()
        {
            if (!current) return false;
            if (!current->render()) current.reset();
            return true;
        }
    };
}
