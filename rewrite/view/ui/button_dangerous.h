#pragma once
#include "button.h"

namespace UI
{
    class Button_Dangerous : public Button
    {
    public:
        Button_Dangerous(std::string id, std::function<void()> callback = []() -> void {})
        : Button(id, callback, ImColor::HSV(7.0f / 7.0f, 0.7f, 0.7f), ImColor::HSV(7.0f / 7.0f, 0.8f, 0.8f), ImColor::HSV(7.0f / 7.0f, 0.9f, 0.9f))
        { }
    };
}