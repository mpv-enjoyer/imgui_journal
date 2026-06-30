#pragma once
#include "model/imodel.h"
#include "commands/icommand.h"
#include <optional>
#include <string>

// Controller doesn't support multiple implementations.
// The only reason to separate the interface is to
// prevent creation of non-const controller anywhere.
class IController
{
public:
    virtual const IModel& model() const = 0;
    virtual void add(std::shared_ptr<ICommand> command) = 0;
    virtual void flush(Time_State::Bits view_time_state) = 0;
};