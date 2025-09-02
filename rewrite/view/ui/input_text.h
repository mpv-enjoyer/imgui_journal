#pragma once
#include "aunit.h"
#include <functional>
#include "../../imgui/misc/cpp/imgui_stdlib.h"

namespace UI
{
    class Input_Text : public AUnit
    {
        std::string m_text;
        std::string m_text_buffer;
        std::function<bool(std::string)> m_callback;
        std::size_t m_max_length;
    public:
        Input_Text(std::string id, std::string text, std::function<bool(std::string)> callback = [](std::string) -> bool { return true; }, std::size_t max_length = 1000)
        : AUnit(id), m_text(text), m_text_buffer(text), m_callback(callback), m_max_length(max_length)
        { }
        void render_logic() override
        {
            if (!ImGui::InputText(m_id.c_str(), &m_text_buffer)) return;
            if (m_text_buffer.size() > m_max_length) m_text_buffer = m_text_buffer.substr(0, m_max_length);
            if (m_callback(m_text_buffer)) m_text = m_text_buffer;
        }
    };
}