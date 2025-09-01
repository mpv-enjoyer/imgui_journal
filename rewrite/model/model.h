#pragma once
#include "journal_year.h"
#include "common/modifiers.h"

class Model
{
    NON_COPYABLE_NOR_MOVABLE(Model);
    Ptr<Journal_Year> m_journal_year = Ptr<Journal_Year>::make(Year::make_current());
public:
    Journal_Year* operator->()
    {
        return m_journal_year.get();
    }
    const Journal_Year* const operator->() const
    {
        return m_journal_year.get();
    }
    Model() { }
    void set_year(Year year)
    {
        m_journal_year.reset(new Journal_Year(year));
    }
};

#undef NON_COPYABLE_NOR_MOVABLE