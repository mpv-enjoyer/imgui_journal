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

std::optional<std::string> Controller_Impl::get_error(const std::shared_ptr<ICommand>& command) const
{
    return command->get_error(model());
}

void Controller_Impl::flush()
{
    while (!m_pending_commands.empty())
    {
        auto error = m_pending_commands.front()->get_error(model());
        if (!error) m_pending_commands.front()->call(Model_Impl::get());
        else log_error(*error);
        m_pending_commands.pop();
        Model_Impl::get()->cache_invalidate();
    }
}
