#pragma once
#include "attendance_student.h"
#include "teachers.h"
#include <optional>

class Attendance_Internal_Lesson
{
public:
    enum class Type
    {
        DRAWING = 0,        // ИЗО
        SCULPTING = 1,      // Лепка
        DESIGN = 2,         // Дизайн
        TECHDRAWING = 3,    // Черчение
        SPECIALCOURSE = 4,  // Спецкурс
    };
private:
    const Type m_lesson_type;
    JTime m_begin;
    JTime m_end;
    std::optional<Vector<Teacher>::Position> m_teacher_pos;
    Vector<Attendance_Student> m_attendance_students;
public:
    Attendance_Internal_Lesson(Type lesson_type, JTime begin, JTime end)
    : m_begin(begin), m_end(end), m_lesson_type(lesson_type)
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
    Type get_lesson_name() const
    {
        return m_lesson_type;
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
    //std::unique_ptr<Attendance_Student>& ref_attendance_student(Vector<Attendance_Student>::Position pos)
    //{
    //    return m_attendance_students.ref(pos);
    //}
    const std::unique_ptr<Attendance_Student>& cref_attendance_student(Vector<Attendance_Student>::Position pos) const
    {
        return m_attendance_students.cref(pos);
    }
    std::unique_ptr<Attendance_Student>& ref_attendance_student(Vector<Attendance_Student>::Position pos)
    {
        return m_attendance_students.ref(pos);
    }
    const Vector<Attendance_Student>& cref_data() const
    {
        return m_attendance_students;
    }
    Vector<Attendance_Student>& ref_data()
    {
        return m_attendance_students;
    }
};