#pragma once
#include <vector>

class Removal_Info
{
    bool m_removed = false;
public:
    Removal_Info() { };
    void remove() { m_removed = true; }
    void restore() { m_removed = false; }
    bool is_removed() const { return m_removed; }
};