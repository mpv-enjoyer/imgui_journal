#pragma once
#include <cinttypes>

enum class Status
{
    //INVALID     = -2, // Crash if encounter those
    NOT_AWAITED = -1,
    NO_DATA     = 0,
    ON_LESSON   = 1,
    WAS_ILL     = 2,
    SKIPPED     = 3,
    WORKED_OUT  = 4
};

class Attendance_Status
{
    Status m_status = Status::NO_DATA;
    int m_discount_id = 0;
public:
    Attendance_Status() { };
    void set(Status status, int discount_id)
    {
        m_status = status;
        m_discount_id = discount_id;
    }
    Status get_status() const
    {
        return m_status;
    }
    int discount_id() const
    {
        return m_discount_id;
    }
};