#pragma once
#include "group.h"
#include <vector>
#include <memory>

namespace UI
{
    class Table_Cell
    {
        Group m_group;
    public:
        Table_Cell(std::vector<IUnit*> units)
        : m_group(units, false)
        { }
        void render()
        {
            m_group.render();
        }
        void update()
        {
            m_group.update();
        }
    };
}