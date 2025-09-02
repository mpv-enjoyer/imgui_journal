#pragma once
#include "iunit.h"

namespace UI
{
    class Table_Column_Config
    {
        std::string m_id;
        ImGuiTableColumnFlags m_flags;
    public:
        explicit Table_Column_Config(std::string id, ImGuiTableColumnFlags flags = 0)
        : m_id(id), m_flags(flags)
        {
        
        }
        void apply()
        {
            ImGui::TableSetupColumn(m_id.c_str(), m_flags);
        }
    };
}