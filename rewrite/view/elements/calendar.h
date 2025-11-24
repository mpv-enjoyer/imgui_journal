#pragma once
#include "view/ui/ui.h"
#include "model/jtime.h"

namespace UI
{
    class Calendar : public AUnit
    {
        Month m_init_month;
        Month m_month;
        std::optional<Mday> m_mday;
        std::function<bool (Mday)> m_filter;
        Table m_table;
        void render_logic() override
        {
            if (m_month.get_from_0() != Month::BEGIN_STUDY_MONTH_FROM_0 && ImGui::Button("<##workout"))
            {
                if (m_month.previous_bounded()) m_mday = {};
            }
            ImGui::SameLine();
            if (ImGui::Button(m_month.get_name().c_str()) && m_month != m_init_month)
            {
                m_mday = {};
                m_month = m_init_month;
            }
            ImGui::SameLine();
            if (m_month.get_from_0() != Month::END_STUDY_MONTH_FROM_0 && ImGui::Button(">##workout"))
            {
                if (m_month.next_bounded()) m_mday = {};
            }
            
        }
    public:
        Calendar(std::string id, Month init_month, std::function<bool (Mday)> filter = nullptr)
        : AUnit(id), m_init_month(init_month), m_month(init_month), m_filter(filter)
        {

        }
    };
}