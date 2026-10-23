#pragma once
#include "journal.h"
#include "prices.h"

class Journal;

namespace Journals
{
    Journal* get_journal(Month month); // Assumption: only one year is active. Other pointers are invalid and will be freed.
    bool is_next_month_in_the_same_study_year(Month month);
    bool is_next_month_in_the_same_study_year(Journal* journal);
    bool is_previous_month_in_the_same_study_year(Month month);
    bool is_previous_month_in_the_same_study_year(Journal* journal);
    Month get_month(Journal* journal);
}