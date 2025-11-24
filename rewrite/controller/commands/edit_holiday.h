#pragma once
#include "icommand.h"

class Edit_Holiday : public ICommand
{
    Mday m_mday;
    std::string m_reason;
public:
    Edit_Holiday(Mday mday, std::string reason)
    : m_mday(mday), m_reason(reason) { }
    Error get_error(const IModel& model) const override
    {
        if (!model->holidays()->get_holiday(m_mday)) return "На этот день не назначен праздник";
        return {};
    }
    void call(IModel& model) override
    {
        model->holidays()->get_holiday(m_mday)->set_reason(m_reason);
    }
};