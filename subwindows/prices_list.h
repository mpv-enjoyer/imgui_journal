#pragma once
#include "subwindows.h"

class Subwindow_Prices_List : public Subwindow
{
    std::vector<std::vector<int>> prices;
    int price_ill = 0;
    int price_skipped = 0;
    bool price_loaded_from_journal = false;
    bool unsaved_changes = false;
public:
    Subwindow_Prices_List(Graphical *_graphical);
    bool show_frame();
};