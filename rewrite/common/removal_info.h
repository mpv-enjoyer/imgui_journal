#pragma once
#include <vector>
#include "per_month.h"

class Removal_Info
{
    bool m_removed = false;
public:
    Removal_Info() { };
    void remove() { m_removed = true; }
    void restore() { m_removed = false; }
    bool is_removed() const { return m_removed; }
};

class Removal_Info_Per_Month
{
    Per_Month<bool> m_removed;
public:
    Removal_Info_Per_Month() { };
    void remove(Month month) { m_removed.set_this_and_after(month, true); }
    void restore(Month month) { m_removed.set_this_and_after(month, false); }
    bool is_removed(Month month) const { return m_removed.get(month); }
    void set_only_this_month(Month month, bool removed) { m_removed.set(month, removed); }
};