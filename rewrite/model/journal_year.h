#pragma once
#include "common/ptr.h"
#include "workouts.h"
#include "holidays.h"
#include "attendance_wdays.h"
#include "teachers.h"
#include "students.h"
#include "prices.h"

class Journal_Year
{
    Ptr<Workouts> m_workouts = Ptr<Workouts>::make();
    Ptr<Holidays> m_holidays = Ptr<Holidays>::make();
    Ptr<Attendance_Wdays> m_attendance_wdays = Ptr<Attendance_Wdays>::make();
    Ptr<Teachers> m_teachers = Ptr<Teachers>::make();
    Ptr<Students> m_students = Ptr<Students>::make();
    Ptr<Prices> m_prices = Ptr<Prices>::make();
public:
    const Year bottom_year;
    explicit Journal_Year(Year year)
    : bottom_year(year) { }
    const Ptr<Workouts>& workouts() const { return m_workouts; }
          Ptr<Workouts>& workouts()       { return m_workouts; }
    const Ptr<Holidays>& holidays() const { return m_holidays; }
          Ptr<Holidays>& holidays()       { return m_holidays; }
    const Ptr<Attendance_Wdays>& attendance_wdays() const { return m_attendance_wdays; }
          Ptr<Attendance_Wdays>& attendance_wdays()       { return m_attendance_wdays; }
    const Ptr<Teachers>& teachers() const { return m_teachers; }
          Ptr<Teachers>& teachers()       { return m_teachers; }
    const Ptr<Students>& students() const { return m_students; }
          Ptr<Students>& students()       { return m_students; }
    const Ptr<Prices>& prices() const { return m_prices; }
          Ptr<Prices>& prices()       { return m_prices; }
};