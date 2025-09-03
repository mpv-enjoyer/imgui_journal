#pragma once
#include "icontroller.h"

class Controller_Impl : public IController
{
    std::queue<Ptr<ICommand>> m_pending_commands;
public:
    const IModel& model() const override;
    std::optional<std::string> add(Ptr<ICommand> command) override;
    std::optional<std::string> get_error(Ptr<ICommand> command) const override;
    void flush() override;
};