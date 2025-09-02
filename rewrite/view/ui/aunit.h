#pragma once
#include "iunit.h"
#include <iostream>

namespace UI
{
    class AUnit : public IUnit
    {
    protected:
        const std::string m_id;
        bool m_enabled;
        float m_width;
        virtual void render_logic() { }
        void log_warning(std::string reason)
        {
            std::cerr << "WARN  [AUnit " << m_id << "]: " << reason << "\n";
        }
        void log_error(std::string reason)
        {
            std::cerr << "ERROR [AUnit " << m_id << "]: " << reason << "\n";
        }
    public:
        AUnit(std::string id, bool enabled = true, float width = 0)
        : m_id(id), m_enabled(enabled), m_width(width)
        {
            update();
        }
        void render() override final
        {
            if (!m_enabled) ImGui::BeginDisabled();
            render_logic();
            if (!m_enabled) ImGui::EndDisabled();
        }
        virtual void update() { }
        virtual void disable() override { m_enabled = false; }
        virtual void enable() override { m_enabled = true; }
        virtual bool is_enabled() override { return m_enabled; }
    };
}