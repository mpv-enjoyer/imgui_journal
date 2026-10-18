#pragma once
#include "journal.h"

class Journal;

namespace Journals
{
    Journal* get_journal(Month month); // Assumption: only current year and some other year are active. Other pointers are invalid and will be freed.
    bool is_next_month_in_the_same_study_year(Month month);
    bool is_next_month_in_the_same_study_year(Journal* journal);
    bool is_previous_month_in_the_same_study_year(Month month);
    bool is_previous_month_in_the_same_study_year(Journal* journal);
    Month get_month(Journal* journal);
}