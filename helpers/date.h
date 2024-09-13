#pragma once
#include "../main.h"

NO_IMPLICIT_CONVERSION_T(int, YearFrom1900);
NO_IMPLICIT_CONVERSION_T(int, YearFrom0);
NO_IMPLICIT_CONVERSION_T(int, MonthFrom0);
struct MonthFrom1 : MonthFrom0
{
    MonthFrom1(int _value) : MonthFrom0(_value)
    {
        value++;
    }
};
NO_IMPLICIT_CONVERSION_T(int, MonthFrom1);
NO_IMPLICIT_CONVERSION_T(int, MdayFrom1);
NO_IMPLICIT_CONVERSION_T(int, Wday);

MdayFrom1 get_number_of_days(MonthFrom0 month, YearFrom0 year);