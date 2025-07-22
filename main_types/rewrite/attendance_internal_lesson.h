#pragma once
#include "attendance_student.h"
#include "jtime.h"
#include "teachers.h"
#include <optional>

class Attendance_Internal_Lesson
{
public:
    enum class Lesson_Name
    {
        DRAWING = 0,        // ИЗО
        SCULPTING = 1,      // Лепка
        DESIGN = 2,         // Дизайн
        TECHDRAWING = 3,    // Черчение
        SPECIALCOURSE = 4,  // Спецкурс
    };
private:
    const Lesson_Name m_lesson_name;
    JTime m_begin;
    JTime m_end;
    std::optional<Vector<Teacher>::Position> m_teacher_pos;
    Vector<Attendance_Student> m_attendance_students;
public:
    Attendance_Internal_Lesson(Lesson_Name lesson_name, JTime begin, JTime end)
    : m_begin(begin), m_end(end), m_lesson_name(lesson_name)
    { }
    void set_time(JTime begin, JTime end)
    {
        m_begin = begin;
        m_end = end;
    }
    JTime get_time_begin() const { return m_begin; }
    JTime get_time_end() const { return m_end; }
    void set_teacher_pos(Vector<Teacher>::Position teacher_pos)
    {
        m_teacher_pos = teacher_pos;
    }
    void reset_teacher_pos()
    {
        m_teacher_pos.reset();
    }
    std::optional<Vector<Teacher>::Position> get_teacher_pos() const
    {
        return m_teacher_pos;
    }
    Lesson_Name get_lesson_name() const
    {
        return m_lesson_name;
    }
    void add_student(Attendance_Student* attendance_student)
    {
        m_attendance_students.push_back(attendance_student);
        // ALL checks must be in Controller so omit even that one
        //for (const auto& attendance_students : m_attendance_students.data())
        //{
        //    if (student_pos.get() == attendance_students->get_student_pos().get())
        //    {
        //        return;
        //    }
        //}
    }
    std::unique_ptr<Attendance_Student>& attendance_student_mut(Vector<Attendance_Student>::Position pos)
    {
        return m_attendance_students.get_mut(pos);
    }
    const std::unique_ptr<Attendance_Student>& attendance_student(Vector<Attendance_Student>::Position pos) const
    {
        return m_attendance_students.get(pos);
    }
    const Vector<Attendance_Student>& get() const
    {
        return m_attendance_students;
    }
};