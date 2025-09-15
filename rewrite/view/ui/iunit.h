#pragma once
#include "common/assert.h"
#include "../../imgui/imgui.h"
#include <string>

namespace UI
{
    class IUnit
    {
    public:
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void disable() = 0;
        virtual void enable() = 0;
        virtual bool is_enabled() = 0;
    };
}