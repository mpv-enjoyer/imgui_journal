#include "subwindow_handler.h"

bool Subwindow_Handler::is_subwindow_opened()
{
    return current.size() != 0;
}

void Subwindow_Handler::open_subwindow(Subwindow *subwindow)
{
    if (subwindow == nullptr) return;
    if (is_subwindow_opened() && !current.back()->allow_ontop()) return;
    current.push_back(subwindow);
}

bool Subwindow_Handler::render_subwindow(Render* render)
{
    if (!is_subwindow_opened()) return false;
    for (auto it = current.begin(); it != current.end(); ++it)
    {
        auto subwindow = *it;
        if (subwindow->show_frame())
        {
            delete subwindow;
            current.erase(it);
            return true;
        }
    }
    return true;
}