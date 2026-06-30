#include "model/model_impl.h"
#include "controller_impl.h"
#include "common/logs.h"
#include <iostream>

const IModel &Controller_Impl::model() const
{
    return Model_Impl::get();
}

void Controller_Impl::add(std::shared_ptr<ICommand> command)
{
    m_pending_commands.push(std::move(command));
}

void Controller_Impl::flush(Time_State::Bits view_time_state)
{
    while (!m_pending_commands.empty())
    {
        auto command = m_pending_commands.front();
        if (!command->is_fitting(view_time_state))
        {
            log_error("is_fitting violation, skipping command!");
        }
        auto error = command->get_error(model());
        if (!error) command->call(Model_Impl::get());
        else log_error(*error);
        m_pending_commands.pop();
        Model_Impl::get()->cache_invalidate();
    }
}
