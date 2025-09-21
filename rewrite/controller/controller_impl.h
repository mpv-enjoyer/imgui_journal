#pragma once
#include "icontroller.h"
#include <queue>

class Controller_Impl : public IController
{
    std::queue<Ptr<ICommand>> m_pending_commands;
public:
    Controller_Impl() { }
    const IModel& model() const override;
    void add(Ptr<ICommand> command) override;
    std::optional<std::string> get_error(Ptr<ICommand> command) const override;
    void flush() override;
};