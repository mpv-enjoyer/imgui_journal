#pragma once
#include <optional>
#include <string>
#include "model/imodel.h"

class IController;

class ICommand
{
public:
    virtual std::optional<std::string> get_error(const IModel&) = 0;
    virtual void call(IModel&) = 0;
};
