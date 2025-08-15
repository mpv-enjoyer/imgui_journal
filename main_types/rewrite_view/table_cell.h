#pragma once
#include "group.h"
#include <vector>
#include <memory>

class Table_Cell
{
    Group m_group;
public:
    Table_Cell(std::vector<IUnit*> units)
    : m_group(units, true, false)
    { }
    void render()
    {
        m_group.render();
    }
};