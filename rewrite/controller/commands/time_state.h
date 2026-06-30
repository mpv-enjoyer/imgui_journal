#pragma once
#include <inttypes.h>
#include "common/jtime.h"

class Time_State
{
public:
    enum Bits : int32_t
    {
        PastYear               = 1 << 0,
        CurrentYearPastMonth   = 1 << 1,
        CurrentMonth           = 1 << 2,
        CurrentYearFutureMonth = 1 << 3,
        FutureYear             = 1 << 4,
        CurrentYear = CurrentYearPastMonth | CurrentMonth | CurrentYearFutureMonth,
    };
    static Bits make(Month month)
    {
        Month current_month = Month::make_current();
        Month current_study_year_begin_month = Month::make_begin_study_year(current_month);
        
        Year next_study_bottom_year = current_month.get_study_bottom_year();
        next_study_bottom_year.next();
        Month next_study_year_begin_month = Month::make_begin_study_year_from_bottom_year(next_study_bottom_year);

        if (month < current_study_year_begin_month) return PastYear;
        if (month < current_month) return CurrentYearPastMonth;
        if (month == current_month) return CurrentMonth;
        if (month < next_study_year_begin_month) return CurrentYearFutureMonth;
        return FutureYear;
    };
};

#define CMD_WANT_STATE(BITSTATE) \
    bool is_fitting(Time_State::Bits state) const override { return IS_FITTING(state); } \
    static bool IS_FITTING(Time_State::Bits state) { return (BITSTATE)&(state); } \
    static_assert(!((BITSTATE) & Time_State::FutureYear));
    