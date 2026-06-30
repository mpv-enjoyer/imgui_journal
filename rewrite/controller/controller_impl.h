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
    void flush(Time_State::Bits view_time_state) override;
    // TODO: set_year(Year year)? Would be cool to flush on it right here.
};