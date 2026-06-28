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

class Removal_Info_Per_Month
{
    std::array<bool, Month::COUNT> m_removed = {};
public:
    Removal_Info_Per_Month() { };
    void remove(Month month) { m_removed[month.calculate_study_year_index()] = true; }
    void restore(Month month) { m_removed[month.calculate_study_year_index()] = false; }
    bool is_removed(Month month) const { return m_removed[month.calculate_study_year_index()]; }
};