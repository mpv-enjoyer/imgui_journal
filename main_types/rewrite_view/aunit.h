#pragma once
#include "iunit.h"

class AUnit : public IUnit
{
protected:
    std::string m_id;
    bool m_enabled;
    float m_width;
    virtual void render_logic() { }
public:
    AUnit(std::string id, bool enabled = true, float width = 0)
    : m_id(id), m_enabled(enabled), m_width(width)
    { }
    void render() override
    {
        if (!m_enabled) ImGui::BeginDisabled();
        render_logic();
        if (!m_enabled) ImGui::EndDisabled();
    }
    virtual void disable() override { m_enabled = false; }
    virtual void enable() override { m_enabled = true; }
    virtual bool is_enabled() override { return m_enabled; }
    virtual void set_id(std::string id) { m_id = id; }
};