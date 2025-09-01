#pragma once
#include "button.h"

class Button_Good : public IUnit
{
    Button m_button;
public:
    Button_Good(std::string id, std::function<void()> callback = []() -> void {})
    : m_button(id, callback, ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f), ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f), ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f))
    { }
    void render() override
    {
        m_button.render();
    }
};