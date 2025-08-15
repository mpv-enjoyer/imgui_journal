#pragma once
#include "table_row.h"

class Table : public AUnit
{
    std::vector<std::string> m_headers;
    std::vector<Table_Row> m_rows;
public:
    Table(std::string id, std::vector<std::string> headers, std::vector<Table_Row> rows)
    : AUnit(id, true, TODO)
    {

    }
protected:
    void render_logic() override
    {

    }
};