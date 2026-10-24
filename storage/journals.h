#pragma once
#include "journal.h"
#include "prices.h"

class Journal;

namespace Journals
{
    Journal* get_journal_cross_year(Month month); // Assumption: only one year is active. Other pointers are invalid and will be freed.
    std::optional<Journal*> get_next_journal_this_year(Journal* journal);
    Journal* get_next_journal_cross_year(Journal* journal);
    std::optional<Journal*> get_previous_journal_this_year(Journal* journal);
    Journal* get_previous_journal_cross_year(Journal* journal);
    Month get_month(Journal* journal);
}