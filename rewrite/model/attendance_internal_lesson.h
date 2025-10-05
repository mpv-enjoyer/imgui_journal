#pragma once
#include "attendance_student.h"
#include "teachers.h"
#include <optional>

enum class Lesson_Type
{
    DRAWING = 0,        // ИЗО
    SCULPTING = 1,      // Лепка
    DESIGN = 2,         // Дизайн
    TECHDRAWING = 3,    // Черчение
    SPECIALCOURSE = 4,  // Спецкурс
};

class Attendance_Internal_Lesson
{
public:
    using Type = Lesson_Type;
private:
    const Type m_lesson_type;
    JTime m_begin;
    JTime m_end;
    std::optional<Vector_Sortable<Teacher>::Position> m_teacher_pos;
    Vector_Sortable<Attendance_Student> m_attendance_students;
public:
    Attendance_Internal_Lesson(Type lesson_type, JTime begin, JTime end)
    : m_lesson_type(lesson_type), m_begin(begin), m_end(end) 
    { }
    void set_time(JTime begin, JTime end)
    {
        m_begin = begin;
        m_end = end;
    }
    JTime get_time_begin() const { return m_begin; }
    JTime get_time_end() const { return m_end; }
    void set_teacher_pos(Vector_Sortable<Teacher>::Position teacher_pos)
    {
        m_teacher_pos = teacher_pos;
    }
    void reset_teacher_pos()
    {
        m_teacher_pos.reset();
    }
    std::optional<Vector_Sortable<Teacher>::Position> get_teacher_pos() const
    {
        return m_teacher_pos;
    }
    Type get_lesson_type() const
    {
        return m_lesson_type;
    }
    void add_student(std::size_t holders_count, Position<Student> student_pos)
    {
        m_attendance_students.push_back(Ptr<Attendance_Student>::make(holders_count, student_pos));
    }
    const Attendance_Student& cref_student(Vector_Sortable<Attendance_Student>::Position pos) const
    {
        return m_attendance_students.cref(pos);
    }
    Attendance_Student& ref_student(Vector_Sortable<Attendance_Student>::Position pos)
    {
        return m_attendance_students.ref(pos);
    }
    const Vector_Sortable<Attendance_Student>& cref_students() const
    {
        return m_attendance_students;
    }
    Vector_Sortable<Attendance_Student>& ref_data()
    {
        return m_attendance_students;
    }
};