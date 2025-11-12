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
    public:
        Input_Int(std::string id, int value = 0, const Updater* updater = nullptr, std::function<bool(int)> callback = [](int) -> bool { return true; }, int min = 0, int max = __INT_MAX__)
        : AUnit(id, updater), m_value(value), m_callback(callback), m_min(min), m_max(max)
        { }
        void render_logic()
        {
            int value_buffer = m_value;
            if (!ImGui::InputInt(m_id.c_str(), &value_buffer)) return;
            if (value_buffer < m_min) value_buffer = m_min;
            if (value_buffer > m_max) value_buffer = m_max;
            if (m_callback(value_buffer)) m_value = value_buffer;
        }
        int get_value() const
        {
            return m_value;
        }
    };
}