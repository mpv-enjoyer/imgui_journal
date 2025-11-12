#pragma once
#include "aunit.h"
#include "common/ptr.h"
#include <vector>
#include <memory>

namespace UI
{
    class Group : public AUnit
    {
        std::vector<Ptr<IUnit>> m_units;
        bool m_engine_group;
    public:
        Group(std::vector<IUnit*> units, const Updater* updater = nullptr, bool engine_group = true)
        : AUnit("", updater), m_engine_group(engine_group)
        {
            for (auto unit : units)
            {
                m_units.emplace_back(unit);
            }
        }
        void update() override
        {
            for (auto& unit : m_units)
            {
                unit->update();
            }
        }
    protected:
        void render_logic() override
        {
            if (m_engine_group) ImGui::BeginGroup();
            for (auto& unit : m_units)
            {
                unit->render();
            }
            if (m_engine_group) ImGui::EndGroup();
        }
    };
}