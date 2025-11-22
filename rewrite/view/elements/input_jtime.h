#pragma once
#include "view/ui/ui.h"
#include "model/jtime.h"

namespace UI
{
    class Input_JTime : public AUnit
    {
        class JTime_Input_Int : public AUnit
        {
            int m_max;
            int m_value;
            std::vector<char> m_buffer;
            void render_logic() override
            {
                UI::Scope_Color_Input color;
                ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888").x);
                if (!ImGui::InputText(m_id.c_str(), m_buffer.data(), m_buffer.size())) return;
                int value_buffer;
                if (std::sscanf(m_buffer.data(), "%i", &value_buffer) == 0) return;
                if (value_buffer < 0) value_buffer = 0;
                if (value_buffer > m_max) value_buffer = m_max;
                m_value = value_buffer;
                std::sprintf(m_buffer.data(), "%02i", m_value);
            }
        public:
            JTime_Input_Int(std::string id, int max, int value) : AUnit(id), m_max(max), m_value(value), m_buffer(std::to_string(max).size() + 1, '\0')
            {
                std::sprintf(m_buffer.data(), "%02i", m_value);
            }
            int get_value() const { return m_value; }
        };
        JTime_Input_Int m_input_hours;
        JTime_Input_Int m_input_minutes;
        void render_logic() override
        {
            if (ImGui::BeginTable(m_id.c_str(), 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
            {
                ImGui::TableNextColumn();
                m_input_hours.render();
                ImGui::TableNextColumn();
                ImGui::Text(":");
                ImGui::TableNextColumn();
                m_input_minutes.render();
                ImGui::EndTable();
            }
        }
    public:
        Input_JTime(std::string id, JTime jtime = {0, 0})
        : AUnit(id + "##INPUT_JTIME"),
          m_input_hours("##1", 23, jtime.get_hours()),
          m_input_minutes("##2", 59, jtime.get_minutes())
        {

        }
        JTime get_value() const
        {
            return JTime(m_input_hours.get_value(), m_input_minutes.get_value());
        }
    };
}