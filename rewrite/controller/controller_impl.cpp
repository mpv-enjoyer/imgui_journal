#include "model/model_impl.h"
#include "controller_impl.h"

const IModel &Controller_Impl::model() const
{
    return Model_Impl::get();
}

void Controller_Impl::add(Ptr<ICommand> command)
{
    // Beware! It's my first time using std move:
    m_pending_commands.push(std::move(command));
}

std::optional<std::string> Controller_Impl::get_error(Ptr<ICommand> command) const
{
    return command->get_error(model());
}

void Controller_Impl::flush()
{
    while (!m_pending_commands.empty())
    {
        auto error = m_pending_commands.front()->get_error(model());
        if (!error) m_pending_commands.front()->call(Model_Impl::get());
        m_pending_commands.pop();
    }
}
