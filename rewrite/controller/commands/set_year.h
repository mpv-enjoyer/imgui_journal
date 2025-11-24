#pragma once
#include "icommand.h"

class Set_Year : public ICommand
{
    Year m_bottom_year;
public:
    Set_Year(Year bottom_year) : m_bottom_year(bottom_year) { }
    std::optional<std::string> get_error(const IModel& model) const override
    {
        if (model->bottom_year == m_bottom_year) return "Год не изменен";
        return {};
    }
    void call(IModel& model) override
    {
        model.set_year(m_bottom_year);
    }
};