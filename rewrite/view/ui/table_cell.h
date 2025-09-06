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
        Table& m_table;
    public:
        Table_Cell(std::shared_ptr<IUnit> unit, Table& table)
        : m_unit(unit), m_table(table)
        { }
        void render()
        {
            m_unit->render();
        }
        Table& get_table()
        {
            return m_table;
        }
    };
}