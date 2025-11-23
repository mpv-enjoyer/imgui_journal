#pragma once
#include "aunit.h"
#include "helpers.h"
#include <functional>

namespace UI
{
    class Checkbox : public AUnit
    {
        bool m_value = false;
        std::function<bool(bool)> m_callback;
        void render_logic() override
        {
            Scope_Color_Input color;
            bool buffer = m_value;
            bool activated = ImGui::Checkbox(m_id.c_str(), &buffer);
            if (activated && (!m_callback || m_callback(buffer))) m_value = buffer;
        }
    public:
        Checkbox(std::string id, std::function<bool(bool)> callback = nullptr)
        : AUnit(id), m_callback(callback)
        {

        }
        bool get_value() const
        {
            return m_value;
        }
    };
}