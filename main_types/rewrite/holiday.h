#pragma once
#include <string>
#include "lib_time.h"

class Holiday
{
    std::string m_reason;
    Mday m_mday;
public:
    explicit Holiday(Mday mday)
    : m_mday(mday)
    { }
    std::string get_reason() const
    {
        return m_reason;
    }
    void set_reason(std::string reason)
    {
        m_reason = reason;
    }
    Mday get_mday() const
    {
        return m_mday;
    }
};