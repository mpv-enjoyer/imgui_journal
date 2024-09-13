#pragma once
#include "../main.h"

struct YearFrom1900;
struct YearFrom0;
struct MonthFrom0;
struct MonthFrom1;
struct MdayFrom0;
struct MdayFrom1;
struct WdayEnglish;
struct WdayRussian;

struct YearFrom1900 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(YearFrom1900);
    operator YearFrom0() const { return value + 1900; };
    YearFrom1900(int _value) : NotImplicitlyConvertable(_value) {}
};

struct YearFrom0 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(YearFrom0);
    operator YearFrom1900() const { return value - 1900; };
    YearFrom0(int _value) : NotImplicitlyConvertable(_value) {}
};

struct MonthFrom0 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(MonthFrom0);
    operator MonthFrom1() const { return value - 1; };
    MonthFrom0(int _value) : NotImplicitlyConvertable(_value) { IM_ASSERT(value < 12 && value >= 0); }
};

struct MonthFrom1 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(MonthFrom1);
    operator MonthFrom0() const { return value + 1; };
    MonthFrom1(int _value) : NotImplicitlyConvertable(_value) { IM_ASSERT(value <= 12 && value > 0); }
};

struct WdayEnglish : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(WdayEnglish);
    operator WdayRussian() const { return LOOP_MINUS(value, 7); };
    WdayEnglish(int _value) : NotImplicitlyConvertable(_value) {}
};

struct WdayRussian : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(WdayRussian);
    operator WdayEnglish() const { return LOOP_PLUS(value, 7); };
    WdayRussian(int _value) : NotImplicitlyConvertable(_value) {}
};

struct MdayFrom0 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(MdayFrom0);
    operator MdayFrom1() const { return value - 1; };
    MdayFrom0(int _value) : NotImplicitlyConvertable(_value) { IM_ASSERT(value < 31 && value >= 0); }
};

struct MdayFrom1 : NotImplicitlyConvertable<int> 
{
    NO_IMPLICIT_CONVERSION_T_COMPARISON(MdayFrom1);
    operator MdayFrom0() const { return value + 1; };
    MdayFrom1(int _value) : NotImplicitlyConvertable(_value) { IM_ASSERT(value <= 31 && value > 0); }
};

NO_IMPLICIT_CONVERSION_T_CHECKED(int, MdayIDWday, value <= 4 && value >= 0);

MdayFrom0 get_number_of_days(MonthFrom0 month, YearFrom0 year);