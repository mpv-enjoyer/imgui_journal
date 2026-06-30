#pragma once
#include "view/ui/ui.h"
#include "common/jtime.h"

namespace UI
{
    class Calendar : public AUnit
    {
        Month m_init_month;
        Month m_month;
        std::optional<Mday> m_mday;
        std::function<bool (Mday)> m_filter;
        std::vector<bool> m_mdays_possible;
        std::function<void (Mday)> m_callback;
        void update_table()
        {
            Mday mday = Mday::make_first(m_month);
            do
            {
                m_mdays_possible.push_back(!m_filter || m_filter(mday));
            } while (mday.next());
            m_mday = {};
        }
        void render_logic() override
        {
            {
                Scope_Disabled disabled(m_month.get_from_0() == Month::BEGIN_STUDY_MONTH_FROM_0);
                if (ImGui::Button("<##workout") && m_month.previous_bounded()) update_table();
                ImGui::SameLine();
            }
            
            {
                Scope_Disabled disabled(m_month == m_init_month);
                if (ImGui::Button(m_month.get_name().c_str(), ImVec2(150, 0))) update_table();
            }
            
            {
                Scope_Disabled disabled(m_month.get_from_0() == Month::END_STUDY_MONTH_FROM_0);
                ImGui::SameLine();
                if (ImGui::Button(">##workout") && m_month.next_bounded()) update_table();
            }
            const static ImGuiTableColumnFlags FLAGS = ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit;
            table("##Календарь", Wday::COUNT, FLAGS, {}, {}, [&]()
            {
                ImGui::TableNextRow();
                Mday mday = Mday::make_first(m_month);
                ImGui::TableSetColumnIndex(Wday::make_from_mday(mday).get_RU());
                bool next_mday_exists = true;
                do
                {
                    bool possible = m_mdays_possible[mday.get_from_0()];
                    if (!possible)
                    {
                        Scope_Disabled disabled;
                        button_selectable("??", false, true);
                    }
                    else
                    {
                        bool selected = (m_mday && *m_mday == mday);
                        if (selected)
                        {
                            button_selectable(std::to_string(mday.get_from_1()), true, true);
                        }
                        else if (button_selectable(std::to_string(mday.get_from_1()), false, true))
                        {
                            m_mday = mday;
                            if (m_callback) m_callback(mday);
                        }
                    }
                    next_mday_exists = mday.next();
                    if (next_mday_exists) ImGui::TableNextColumn();
                } while (next_mday_exists);
            });
        }
    public:
        Calendar(std::string id, Month init_month, std::function<bool (Mday)> filter = nullptr, std::function<void (Mday)> callback = nullptr)
        : AUnit(id), m_init_month(init_month), m_month(init_month), m_filter(filter), m_callback(callback)
        {
            update_table();
        }
    };
}