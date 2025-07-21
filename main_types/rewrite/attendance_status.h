#pragma once
#include <cinttypes>

class Attendance_Status
{
    int m_status = 0;
    int m_discount_id = 0;
public:
    class Status
    {
        const int m_value;
        Status(int value) : m_value(value) { }
    public:
        static Status INVALID = Status(-2);
        static Status make_not_awaited() { return Status(-1); }
        static Status make_no_data() { return Status(0); }
        static Status make_on_lesson() { return Status(1); }
        static Status make_invalid() { return Status(2); }
        static Status make_invalid() { return Status(3); }
    }; // TODO: enum here?
    static const int STATUS_INVALID     = -2; // Mostly unused
    static const int STATUS_NOT_AWAITED = -1; // Must be assigned on later layers
    static const int STATUS_NO_DATA     =  0;
    static const int STATUS_ON_LESSON   =  1;
    static const int STATUS_WAS_ILL     =  2;
    static const int STATUS_SKIPPED     =  3;
    static const int STATUS_WORKED_OUT  =  4; // Must be assigned on workout layer
    Attendance_Status();
    void set(int status, int discount_id)
    {
        m_status = status;
        m_discount_id = discount_id;
    }
};