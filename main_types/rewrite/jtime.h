#pragma once

class JTime //used separately with ctime.
{
    int m_hours; //0-23
    int m_minutes; //0-59
    int bounded(int value, int max)
    {
        if (value >= max) return max - 1;
        if (value < 0) return 0;
        return value;
    }
public:
    JTime(int hours, int minutes)
    : m_hours(bounded(hours, 24)),
      m_minutes(bounded(minutes, 60))
    { }
    int get_minutes() const { return m_minutes; }
    int get_hours() const { return m_hours; }
};