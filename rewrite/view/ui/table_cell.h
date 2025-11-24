#pragma once
#include "group.h"
#include <vector>
#include <memory>

namespace UI
{
    class Table;

    class Table_Cell
    {
        // using shared_ptr instead of Ptr<> because I want Table_Cell
        // to be copyable for Table_Row and Table initialization.
        std::shared_ptr<IUnit> m_unit;
    public:
        Table_Cell(std::shared_ptr<IUnit> unit)
        : m_unit(unit)
        { }
        void render()
        {
            if (m_unit) m_unit->render();
        }
    };
}