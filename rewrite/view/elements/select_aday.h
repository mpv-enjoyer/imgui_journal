#pragma once
#include "view/ui/ui.h"
#include "model/jtime.h"
#include "common/logs.h"
#include <algorithm>

namespace UI
{
    class Select_Aday : public AUnit
    {
        Combobox<Aday> m_combobox;
        static Combobox_Choices<Aday> get_choices(Year bottom_year, Mday mday, Aday min, Aday max_inc)
        {
            Wday wday = Wday::make_from_mday(mday);
            //Month::make_begin_study_year_from_bottom_year(bottom_year);
            Aday current = Aday::make_from_mday(mday);
            if (current.index() > max_inc.index() || current.index() < min.index())
            {
                log_warning("Current mday " + mday.to_string() + " is not in aday range! Falling back to min");
                current = min;
            }
            Combobox_Choices<Aday> output;
            for (Aday i = min; i.index() <= max_inc.index(); i.next())
            {
                output.emplace_back(Mday::make_from_aday(bottom_year, wday, i).to_string(), i);
            }
            std::reverse(output.begin(), output.end());
            return output;
        }
        void render_logic() override
        {
            m_combobox.render();
        }
    public:
        Select_Aday(std::string id, Mday mday, Aday min, Aday max_inc)
        : AUnit(id), m_combobox(id, get_choices(mday.get_month().get_study_bottom_year(), mday, min, max_inc), max_inc.index() - Aday::make_from_mday(mday).index())
        { }
        Aday get_choice() const
        {
            return m_combobox.get_choice();
        }
    };
}