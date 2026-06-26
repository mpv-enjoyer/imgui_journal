#pragma once
#include <vector>

TODO_CRITICAL(Keep an internal track of months)
class Removal_Info
{
    bool m_removed = false;
public:
    Removal_Info() { };
    void remove() { m_removed = true; }
    // remove_one_month(Month month)
    void restore() { m_removed = false; }
    // restore_one_month(Month month)
    bool is_removed(/* Month */) const { return m_removed; }
};