#pragma once
#include "aunit.h"
#include <functional>
#include <vector>

namespace UI
{
    template <typename ChoicesT>
    class Combobox : public AUnit
    {
        std::string m_choices;
    	int m_value;
    	std::function<bool(ChoicesT)> m_callback;
    public:
        Combobox(std::string id, std::vector<std::string> choices, ChoicesT value = 0, std::function<bool(ChoicesT)> callback = [](ChoicesT){ return true; })
        : AUnit(id), m_value(value), m_callback(callback)
        {
            if (choices.size() != ChoicesT::COUNT)
            {
                log_warning("Combobox choices size doesn't match ChoicesT::COUNT");
            }
            for (auto choice : choices)
            {
                m_choices.append(choice + std::string('\0'));
            }
            m_choices.append('\0');
        }
        void render_logic() override
        {
            int value_buffer = m_value;
            if (!ImGui::Combo(m_id.c_str(), &value_buffer, m_choices.c_str())) return;
            if (m_callback(value_buffer)) m_value = value_buffer;
        }
    };
}