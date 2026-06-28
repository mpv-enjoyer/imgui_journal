#pragma once
#include <optional>
#include <string>
#include "model/imodel.h"

enum class Time_State
{
    PastYear               = 1 << 0,
    CurrentYearPastMonth   = 1 << 1,
    CurrentMonth           = 1 << 2,
    CurrentYearFutureMonth = 1 << 3,
    FutureYear             = 1 << 4,
    CurrentYear = CurrentYearPastMonth | CurrentMonth | CurrentYearFutureMonth,
};

class ICommand
{
public:
    // static bool IS_FITTING(Time_State state) { ... };
    virtual bool is_fitting(Time_State state) const = 0;
    // get_error should not be expensive. That is because
    // it will be called 1 or 2 times before the call actually happens:
    using Error = std::optional<std::string>;
    virtual Error get_error(const IModel&) const = 0;
    virtual void call(IModel&) = 0;
};