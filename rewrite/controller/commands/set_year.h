#pragma once
#include "icommand.h"

class Set_Year_From_Month : public ICommand
{
    Month m_month;
public:
    Set_Year_From_Month(Month month) : m_month(month) { }
    std::optional<std::string> get_error(const IModel& model) const override
    {
        if (model->is_within_current(m_month)) return "Год не изменен";
        return {};
    }
    void call(IModel& model) override
    {
        model.set_year(m_month.get_study_bottom_year());
    }
};