#pragma once
#include "table_cell.h"
#include <vector>

namespace UI
{
    class Table_Row
    {
        std::vector<Table_Cell> m_cells;
        std::size_t m_id; // for resorting
    public:
        Table_Row(std::vector<Table_Cell> cells)
        : m_cells(cells)
        { }
        void render()
        {
            for (std::size_t i = 0; i < m_cells.size(); i++)
            {
                ImGui::TableSetColumnIndex(i);
                m_cells[i].render();
            }
        }
        void update()
        {
            for (auto& cell : m_cells)
            {
                cell.update();
            }
        }
        std::size_t size()
        {
            return m_cells.size();
        }
    };
}