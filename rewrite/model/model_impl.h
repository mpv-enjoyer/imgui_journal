#pragma once
#include "imodel.h"
#include <queue>

class Model_Impl : public IModel
{
    NON_COPYABLE_NOR_MOVABLE(Model_Impl);
    static Model_Impl* m_instance_ptr;
    Ptr<Journal_Year> m_journal_year;
    Model_Impl()
    : m_journal_year(Ptr<Journal_Year>::make(Year::make_current()))
    {
        /* TODO CRITICAL: Implement load current month by default using Loader */
    }
public:
    static Model_Impl& get()
    {
        if (!m_instance_ptr) m_instance_ptr = new Model_Impl();
        return *m_instance_ptr;
    }
    Journal_Year* const operator->() override
    {
        return m_journal_year.get();
    }
    const Journal_Year* const operator->() const override
    {
        return m_journal_year.get();
    }
    Journal_Year& operator*() override
    {
        return *m_journal_year;
    }
    const Journal_Year& operator*() const override
    {
        return *m_journal_year;
    }
    void set_year(Year year) override
    {
        /* TODO CRITICAL: Actually implement year change with saving using Loader */
    }
    void save() override
    {
        /* TODO CRITICAL: Save using Loader */
    }
};
