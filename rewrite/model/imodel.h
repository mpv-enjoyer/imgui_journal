#pragma once
#include "journal_year.h"
#include "common/modifiers.h"
#include "helpers.h"

class IModel
{
public:
    virtual Journal_Year* const operator->() = 0;
    virtual const Journal_Year* const operator->() const = 0;
    virtual Journal_Year& const operator*() = 0;
    virtual const Journal_Year& const operator*() const = 0;
    virtual void set_year(Year year) = 0;
    virtual void save() = 0;
    /* TODO CRITICAL: saveload using some Loader */
};