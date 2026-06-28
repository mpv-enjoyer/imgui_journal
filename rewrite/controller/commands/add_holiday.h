#pragma once
#include "icommand.h"

class Add_Holiday : public ICommand
{
    Mday m_mday;
    std::string m_reason;
public:
    Add_Holiday(Mday mday, std::string reason)
    : m_mday(mday), m_reason(reason) { }

    static bool IS_FITTING(Time_State state) { return state == Time_State::CurrentYear; }
    bool is_fitting(Time_State state) const override { return IS_FITTING(state); }
    TODO_CRITICAL(Do the same for all other actions. Its kinda weird but I need extra validation);
    
    Error get_error(const IModel& model) const override
    {
        if (model->holidays()->get_holiday(m_mday)) return "На этот день уже назначен праздник";
        return {};
    }
    void call(IModel& model) override
    {
        Holiday holiday(m_mday);
        holiday.set_reason(m_reason);
        model->holidays()->add_holiday(holiday);
    }
};