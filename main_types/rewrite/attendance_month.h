#pragma once
#include "attendance_status.h"
#include "lib_vector.h"
#include "lib_time.h"

class Attendance_Month
{
    std::vector<Attendance_Status> mday_statuses;
public:
    Attendance_Month(Month month, Wday wday)
    : mday_statuses(std::vector<Attendance_Status>(month.calculate_wday_count(wday)))
    { }
    
};