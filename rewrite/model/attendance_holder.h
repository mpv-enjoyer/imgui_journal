#pragma once
#include <cinttypes>

enum class Attendance_Status
{
    //INVALID     = -2, Crash if encounter those
    NOT_AWAITED = -1, // Don't use. Use Holidays, Attendance_Student::m_wants_lesson, ...?
    NO_DATA     = 0,
    ON_LESSON   = 1,
    WAS_ILL     = 2,
    SKIPPED     = 3,
    //WORKED_OUT  = 4 Store workouts in Workouts class instead.
};

class Attendance_Holder
{
    Attendance_Status m_status = Attendance_Status::NO_DATA;
    int m_discount_id = 0;
public:
    Attendance_Holder() { };
    void set(Attendance_Status status, int discount_id)
    {
        m_status = status;
        m_discount_id = discount_id;
    }
    Attendance_Status get_status() const
    {
        return m_status;
    }
    // Can go out of bounds. Check 
    int get_discount_id() const
    {
        return m_discount_id;
    }
};