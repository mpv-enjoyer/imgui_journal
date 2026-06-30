#pragma once
#include "icommand.h"

class Remove_Holiday : public ICommand
{
    Mday m_mday;
    std::string m_reason;
public:
    Remove_Holiday(Mday mday, std::string reason)
    : m_mday(mday), m_reason(reason) { }

    CMD_WANT_STATE(Time_State::CurrentYear)

    Error get_error(const IModel& model) const override
    {
        if (!model->holidays()->get_holiday(m_mday)) return "На этот день не назначен праздник";
        return {};
    }
    void call(IModel& model) override
    {
        model->holidays()->remove_holiday(m_mday);
    }
};