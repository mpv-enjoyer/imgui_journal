#pragma once
#include "aunit.h"
#include <vector>
#include <memory>

class Group : public AUnit
{
    std::vector<std::unique_ptr<IUnit>> m_units;
    bool m_engine_group;
public:
    Group(std::vector<IUnit*> units, bool enabled = true, bool engine_group = true)
    : AUnit("", enabled), m_engine_group(engine_group)
    {
        for (auto unit : units)
        {
            m_units.emplace_back(unit);
        }
    }
protected:
    void render_logic() override
    {
        for (auto& unit : m_units)
        {
            unit->render();
        }
    }
};