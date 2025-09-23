#pragma once
#include "common/ptr.h"
#include "holidays.h"
#include "attendance_wdays.h"
#include "teachers.h"
#include "students.h"

class Merged_Lesson_ID
{
    Wday m_wday;
    Position<Attendance_Merged_Lesson> m_merged_lesson_pos;
public:
    Merged_Lesson_ID(Wday wday_, Position<Attendance_Merged_Lesson> merged_lesson_pos_)
    : m_wday(wday_), m_merged_lesson_pos(merged_lesson_pos_) { }
    Wday wday() const { return m_wday; }
    Position<Attendance_Merged_Lesson> pos() const { return m_merged_lesson_pos; }
};

class Internal_Lesson_ID
{
    Merged_Lesson_ID m_merged_lesson_id;
    Position<Attendance_Internal_Lesson> m_internal_lesson_pos;
public:
    Internal_Lesson_ID(Merged_Lesson_ID merged_lesson_id, Position<Attendance_Internal_Lesson> internal_lesson_pos)
    : m_merged_lesson_id(merged_lesson_id), m_internal_lesson_pos(internal_lesson_pos) { }
    Wday wday() const { return m_merged_lesson_id.wday(); }
    Position<Attendance_Merged_Lesson> merged_lesson_pos() const { return m_merged_lesson_id.pos(); }
    Position<Attendance_Internal_Lesson> pos() const { return m_internal_lesson_pos; }
    Merged_Lesson_ID merged_lesson_id() const { return m_merged_lesson_id; };
};

class Internal_Student_ID
{
    Internal_Lesson_ID m_internal_lesson_id;
    Position<Attendance_Student> m_attendance_student_pos;
public:
    Internal_Student_ID(Internal_Lesson_ID internal_lesson_id, Position<Attendance_Student> attendance_student_pos)
    : m_internal_lesson_id(internal_lesson_id), m_attendance_student_pos(attendance_student_pos) { }
    Wday wday() const { return m_internal_lesson_id.wday(); }
    Position<Attendance_Merged_Lesson> merged_lesson_pos() const { return m_internal_lesson_id.merged_lesson_pos(); }
    Position<Attendance_Internal_Lesson> internal_lesson_pos() const { return m_internal_lesson_id.pos(); }
    Position<Attendance_Student> pos() const { return m_attendance_student_pos; }
    Internal_Lesson_ID internal_lesson_id() const { return m_internal_lesson_id; }
};

class Attendance_ID
{
    Internal_Student_ID m_internal_student_id;
    Mday m_mday;
public:
    Attendance_ID(Internal_Student_ID internal_student_id, Mday mday)
    : m_internal_student_id(internal_student_id), m_mday(mday)
    {
        IM_ASSERT(Wday::make_from_mday(mday) == m_internal_student_id.wday());
    }
    Wday wday() const { return Wday::make_from_mday(m_mday); }
    Position<Attendance_Merged_Lesson> merged_lesson_pos() const { return m_internal_student_id.merged_lesson_pos(); }
    Position<Attendance_Internal_Lesson> internal_lesson_pos() const { return m_internal_student_id.internal_lesson_pos(); }
    Position<Attendance_Student> attendance_student_pos() const { return m_internal_student_id.pos(); }
    Mday mday() const { return m_mday; }
    Internal_Student_ID internal_student_id() const { return m_internal_student_id; }
};

class Workout_Lesson_ID
{
    Attendance_ID m_should_id;
    Internal_Lesson_ID m_real_internal_lesson_id;
    
    //Mday mday;
    //Position<Attendance_Merged_Lesson> merged_lesson_pos;
    //Position<Attendance_Internal_Lesson> internal_lesson_pos;
    //bool operator==(const Workout_Lesson_ID& other) const
    //{
    //    return std::tie(this->mday, this->merged_lesson_pos, this->internal_lesson_pos)
    //        == std::tie(other.mday, other.merged_lesson_pos, other.internal_lesson_pos);
    //}
    //bool operator!=(const Workout_Lesson_ID& other) const
    //{
    //    return !(*this == other);
    //}
};