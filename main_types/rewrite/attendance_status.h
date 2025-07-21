#pragma once

using size_t = unsigned long long; //idk

class Attendance_Status
{
    int status = 0;
    size_t discount_id = 0;
public:
    static const int STATUS_INVALID     = -2; // Mostly unused
    static const int STATUS_NOT_AWAITED = -1; // Must be assigned on later layers
    static const int STATUS_NO_DATA     =  0;
    static const int STATUS_ON_LESSON   =  1;
    static const int STATUS_WAS_ILL     =  2;
    static const int STATUS_SKIPPED     =  3;
    static const int STATUS_WORKED_OUT  =  4;
    Attendance_Status();
    void set()
}