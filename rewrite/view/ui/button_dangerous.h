#pragma once
#include "button.h"

namespace UI
{
    class Button_Dangerous : public IUnit
    {
        Button m_button;
    public:
        Button_Dangerous(std::string id, std::function<void()> callback = []() -> void {})
        : m_button(id, callback, ImColor::HSV(7.0f / 7.0f, 0.7f, 0.7f), ImColor::HSV(7.0f / 7.0f, 0.8f, 0.8f), ImColor::HSV(7.0f / 7.0f, 0.9f, 0.9f))
        { }
        void render() override
        {
            m_button.render();
        }
    };
}