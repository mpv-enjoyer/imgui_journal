#pragma once
#include "iunit.h"
#include <iostream>

namespace UI
{
    class Updater
    {
        NON_COPYABLE_NOR_MOVABLE(Updater);
        bool m_value = true;
        bool m_requested_update = false;
    public:
        Updater() { }
        void begin_frame()
        {

        }
        void request_update()
        {
            m_requested_update = true;
        }
        void end_frame()
        {
            if (m_value) m_value = false;
            if (m_requested_update)
            {
                m_value = true;
                m_requested_update = false;
            }
        }
        bool wants_update() const
        {
            return m_value;
        }
    };

    class AUnit : public IUnit
    {
        NON_COPYABLE(AUnit);
    protected:
        const std::string m_id;
        const Updater* const m_updater;
        bool m_enabled = true;
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
        AUnit(std::string id, const Updater* updater = nullptr, float width = 0)
        : m_id(id), m_updater(updater), m_width(width)
        { }
        void render() override final
        {
            if (m_updater && m_updater->wants_update()) update();
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