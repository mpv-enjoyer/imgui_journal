#include "subwindows.h"

void Subwindow::update_graphical(Graphical *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
}