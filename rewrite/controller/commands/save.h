#pragma once
#include "icommand.h"

class Save : public ICommand
{
public:
    Save() { }

    CMD_WANT_STATE(Time_State::CurrentYear)

    std::optional<std::string> get_error(const IModel& model) const override
    {
        return {};
    }
    void call(IModel& model) override
    {
        model.save();
    }
};