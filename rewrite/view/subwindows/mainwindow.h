#pragma once
#include "subwindow.h"
#include "view/shared.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        Shared& m_shared;
        bool render_logic() override
        {
            UNFINISHED
        }
    public:
        Mainwindow(IController& controller, Shared& shared)
        : Subwindow(controller), m_shared(shared)
        { }
    };
}