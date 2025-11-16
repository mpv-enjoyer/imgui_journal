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
    virtual void add(Ptr<ICommand> command) = 0;
    virtual std::optional<std::string> get_error(Ptr<ICommand> command) const = 0;
    virtual void flush() = 0;
};