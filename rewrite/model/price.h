#pragma once
#include <string>
#include "common/modifiers.h"
#include "thirdparty/decimal.h"

class Price
{
    dec::decimal2 m_value;
public:
    using Value_Type = dec::decimal2;
    Price(Value_Type value) : m_value(value) { }
    Value_Type get() const { return m_value; }
};