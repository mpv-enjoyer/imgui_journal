#pragma once
#include "../../imgui/imgui.h"
#include <string>

class IUnit
{
public:
    virtual void render() = 0;
    virtual void disable() = 0;
    virtual void enable() = 0;
    virtual bool is_enabled() = 0;
    virtual void set_id(std::string id) = 0;
};