#pragma once
#include "aunit.h"
#include <functional>

namespace UI
{
    // Example usage:
    // int value = 1;
    // Command c([](int* value) { ImGui::BeginChild("", *value); }, &value, [](int* value) { ImGui::EndChild(); });
    // Only use if nothing else fits (might be entirely useless)
    template <typename T>
    class Command : public AUnit
    {
        T* m_user_data;
        std::function<void(T*)> m_command;
    public:
        [[nodiscard]] Command(std::function<void(T*)> command, T* user_data)
        : AUnit(""), m_command(command), m_user_data(user_data)
        { }
        void render_logic() override
        {
            m_command(m_user_data);
        }
        ~Command()
        {
        
        }
    };
}