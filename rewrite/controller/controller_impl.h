#pragma once
#include "icontroller.h"
#include <queue>

class Controller_Impl : public IController
{
    std::queue<std::shared_ptr<ICommand>> m_pending_commands;
public:
    Controller_Impl() { }
    const IModel& model() const override;
    void add(std::shared_ptr<ICommand> command) override;
    std::optional<std::string> get_error(const std::shared_ptr<ICommand>& command) const override;
    void flush() override;
};