#pragma once
#include <string>
#include "common/modifiers.h"

class Price
{
    int m_kopecks;
public:
    int get_kopecks() const { return m_kopecks; }
    std::string to_string() const
    {
        return "TODO CRITICAL.";
        //TODO_CRITICAL("Use this: github.com/vpiotr/decimal_for_cpp");
    }
};