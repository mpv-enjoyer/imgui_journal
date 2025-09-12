#pragma once
#include "icommand.h"

class Save : public ICommand
{
public:
    Save() { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        return {};
    }
    void call(IModel& model) override
    {
        model.save();
    }
};