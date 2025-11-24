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
        Group(std::vector<IUnit*> units, bool engine_group = true)
        : AUnit(""), m_engine_group(engine_group)
        {
            for (auto unit : units)
            {
                m_units.emplace_back(unit);
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