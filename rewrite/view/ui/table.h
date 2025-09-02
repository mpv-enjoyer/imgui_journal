#pragma once
#include "table_row.h"
#include "table_column_config.h"

namespace UI
{
    class Table : public AUnit
    {
        std::size_t m_column_count;
        std::vector<Table_Column_Config> m_headers;
        std::vector<Table_Row> m_rows;
        ImGuiTableFlags m_flags;
        ImVec2 m_outer_size;
        float m_inner_width;
    public:
        Table(std::string id, std::vector<Table_Column_Config> headers, std::vector<Table_Row> rows, ImGuiTableFlags flags = 0, ImVec2 outer_size = ImVec2((0.0F), (0.0F)), float inner_width = (0.0F))
        : AUnit(id), m_column_count(headers.size()), m_headers(headers), m_rows(rows), m_flags(flags), m_outer_size(outer_size), m_inner_width(inner_width)
        {
            if (m_column_count == 0 && m_rows.size() > 0) m_column_count = m_rows.front().size();
            for (auto row : m_rows)
            {
                if (row.size() != m_column_count)
                {
                    log_warning("Row size != Header row size");
                }
            }
        }
        void update() override
        {
            for (auto row : m_rows)
            {
                row.update();
            }
        }
    protected:
        void render_logic() override
        {
            if (!ImGui::BeginTable(m_id.c_str(), m_column_count, m_flags, m_outer_size, m_inner_width)) return;
            for (auto header : m_headers)
            {
                header.apply();
            }
            for (auto row : m_rows)
            {
                row.render();
                ImGui::TableNextRow();
            }
            ImGui::EndTable();
        }
    };
}