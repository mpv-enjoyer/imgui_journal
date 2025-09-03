#pragma once
#include "journal_year.h"
#include "common/modifiers.h"

class IModel
{
public:
    virtual Journal_Year* const operator->();
    virtual const Journal_Year* const operator->() const = 0;
    void set_year(Year year);
};