#pragma once

#include "journal.h"

struct Payment
{
    uint64_t kopecks;
    /* something here */
};

class Payments
{
    std::unordered_map<int, std::vector<std::vector<Payment>>> payments; // [contract]:[month from STUDY_YEAR_BEGIN_MONTH]
    std::vector<Journal> journals; // [month from STUDY_YEAR_BEGIN_MONTH] (keep that in )
};

// Very first draft - need to do more cross-month actions first.