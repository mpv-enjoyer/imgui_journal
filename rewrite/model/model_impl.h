#pragma once
#include "imodel.h"
#include <queue>

class Model_Impl : public IModel
{
    NON_COPYABLE_NOR_MOVABLE(Model_Impl);
    static Model_Impl* m_instance_ptr;
    Model_Impl()
    {
        /* Implement load current month by default using Loader */
    }
public:
    static Model_Impl& get()
    {
        if (!m_instance_ptr) m_instance_ptr = new Model_Impl();
        return *m_instance_ptr;
    }
    Model_Impl(const Model_Impl& obj) = delete;
    Journal_Year* const operator->() override
    {
        return get().operator->();
    }
    const Journal_Year* const operator->() const override
    {
        return get().operator->();
    }
    void set_year(Year year) override
    {
        /* Actually implement year change with saving using Loader */
    }
};
