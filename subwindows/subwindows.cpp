#include "subwindows.h"

Subwindow::Subwindow(JournalHolder *_graphical, Popup_Handler *_popup_handler)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    popup_handler = _popup_handler;
}

void Subwindow::update_graphical(JournalHolder *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
}