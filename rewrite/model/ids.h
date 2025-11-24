#pragma once
#include "common/ptr.h"
#include "holidays.h"
#include "attendance_merged_lesson.h"
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
    AUTOEQ2(Merged_Lesson_ID, m_wday, m_merged_lesson_pos);
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
    Merged_Lesson_ID merged_lesson_id() const { return m_merged_lesson_id; }
    AUTOEQ2(Internal_Lesson_ID, m_merged_lesson_id, m_internal_lesson_pos);
    operator Merged_Lesson_ID() const { return merged_lesson_id(); }
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
    Merged_Lesson_ID merged_lesson_id() const { return m_internal_lesson_id.merged_lesson_id(); }
    Internal_Lesson_ID internal_lesson_id() const { return m_internal_lesson_id; }
    AUTOEQ2(Internal_Student_ID, m_internal_lesson_id, m_attendance_student_pos);
    operator Merged_Lesson_ID() const { return merged_lesson_id(); }
    operator Internal_Lesson_ID() const { return internal_lesson_id(); }
};

class Attendance_ID
{
    Internal_Student_ID m_internal_student_id;
    Aday m_aday;
public:
    Attendance_ID(Internal_Student_ID internal_student_id, Aday aday)
    : m_internal_student_id(internal_student_id), m_aday(aday) { }
    Wday wday() const { return m_internal_student_id.wday(); }
    Position<Attendance_Merged_Lesson> merged_lesson_pos() const { return m_internal_student_id.merged_lesson_pos(); }
    Position<Attendance_Internal_Lesson> internal_lesson_pos() const { return m_internal_student_id.internal_lesson_pos(); }
    Position<Attendance_Student> attendance_student_pos() const { return m_internal_student_id.pos(); }
    Aday aday() const { return m_aday; }
    Internal_Student_ID internal_student_id() const { return m_internal_student_id; }
    Merged_Lesson_ID merged_lesson_id() const { return m_internal_student_id.merged_lesson_id(); }
    Internal_Lesson_ID internal_lesson_id() const { return m_internal_student_id.internal_lesson_id(); }
    AUTOEQ2(Attendance_ID, m_internal_student_id, m_aday);
    operator Merged_Lesson_ID() const { return merged_lesson_id(); }
    operator Internal_Lesson_ID() const { return internal_lesson_id(); }
    operator Internal_Student_ID() const { return internal_student_id(); }
};
