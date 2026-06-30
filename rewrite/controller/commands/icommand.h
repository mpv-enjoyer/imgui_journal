#pragma once
#include <optional>
#include <string>
#include "model/imodel.h"
#include "time_state.h"

class ICommand
{
public:
    virtual bool is_fitting(Time_State::Bits state) const = 0;
    // get_error should not be expensive. That is because
    // it will be called 1 or 2 times before the call actually happens:
    using Error = std::optional<std::string>;
    virtual Error get_error(const IModel&) const = 0;
    virtual void call(IModel&) = 0;
};