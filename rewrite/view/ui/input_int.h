#pragma once
#include "aunit.h"
#include <functional>

namespace UI
{
    class Input_Int : public AUnit
    {
        int m_value;
        std::function<bool(int)> m_callback;
        int m_min;
        int m_max;
        virtual void update_visible_value()
        {
            std::sprintf(m_buffer.data(), "%i", m_value);
        }
    protected:
        std::vector<char> m_buffer;
    public:
        Input_Int(std::string id, int value = 0, const Updater* updater = nullptr, std::function<bool(int)> callback = nullptr, int min = 0, int max = __INT_MAX__)
        : AUnit(id, updater), m_value(value), m_callback(callback), m_min(min), m_max(max), m_buffer(std::to_string(m_max).size() + 1, '\0')
        {
            update_visible_value();
        }
        void render_logic()
        {
            UI::Scope_Color_Input color;
            if (!ImGui::InputText(m_id.c_str(), m_buffer.data(), m_buffer.size(), ImGuiInputTextFlags_AutoSelectAll)) return;
            int value_buffer;
            if (std::sscanf(m_buffer.data(), "%i", &value_buffer) == 0) return;
            if (value_buffer < m_min) value_buffer = m_min;
            if (value_buffer > m_max) value_buffer = m_max;
            if (!m_callback || m_callback(value_buffer)) m_value = value_buffer;
            update_visible_value();
        }
        int get_value() const
        {
            return m_value;
        }
    };
}