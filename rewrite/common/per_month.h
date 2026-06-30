#pragma once
#include "jtime.h"

template <typename T>
class Per_Month
{
    std::array<T, Month::COUNT> m_removed = {};
    size_t index(Month month) const { return month.calculate_study_year_index(); }
public:
    Per_Month() { };
    void set(Month month, T value) { m_removed[index(month)] = value; }
    void set_this_and_after(Month month, T value)
    {
        do
        {
            set(month, value);
        } while (month.next());
    }
    void set_all(T value)
    {
        for (int i = 0; i < Month::COUNT; i++)
        {
            m_removed[i] = value;
        }
    }
    const T& get(Month month) const { return m_removed[index(month)]; }
};