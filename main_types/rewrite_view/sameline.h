#pragma once
#include "aunit.h"

class Sameline : public AUnit
{
    float m_offset_from_start_x;
    float m_spacing;
public:
    Sameline(float offset_from_start_x = (0.0F), float spacing = (-1.0F))
    : AUnit(""), m_offset_from_start_x(offset_from_start_x), m_spacing(spacing)
    { }
    void render_logic() override
    {
        ImGui::SameLine(m_offset_from_start_x, m_spacing);
    }
};