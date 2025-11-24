#pragma once
#include "view/ui/ui.h"
#include "model/jtime.h"

namespace UI
{
    class Input_JTime : public AUnit
    {
        class JTime_Input_Int : public Input_Int
        {
            void update_visible_value() override
            {
                std::sprintf(m_buffer.data(), "%.2i", get_value());
            }
        public:
            JTime_Input_Int(std::string id, int max, int value)
            : Input_Int(id, value, nullptr, 0, max)
            {
                update_visible_value();
            }
        };
        JTime_Input_Int m_input_hours;
        JTime_Input_Int m_input_minutes;
        void render_logic() override
        {
            float items_width = ImGui::CalcTextSize("8888").x;
            if (ImGui::BeginTable(m_id.c_str(), 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
            {
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(items_width);
                m_input_hours.render();
                ImGui::TableNextColumn();
                label(":");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(items_width);
                m_input_minutes.render();
                ImGui::EndTable();
            }
        }
    public:
        Input_JTime(std::string id, JTime jtime = {0, 0})
        : AUnit(id + "##INPUT_JTIME"),
          m_input_hours("##1", 23, jtime.get_hours()),
          m_input_minutes("##2", 59, jtime.get_minutes())
        { }
        JTime get_value() const
        {
            return JTime(m_input_hours.get_value(), m_input_minutes.get_value());
        }
    };
}