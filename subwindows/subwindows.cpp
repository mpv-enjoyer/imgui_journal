#include "subwindows.h"

Subwindow::Subwindow(Graphical *_graphical, Popup_Handler *_popup_handler)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    popup_handler = _popup_handler;
}

void Subwindow::update_graphical(Graphical *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
}