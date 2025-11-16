#pragma once
#include "attendance_student.h"
#include "teachers.h"
#include "price.h"
#include <optional>

enum class Lesson_Type
{
    DRAWING = 0,        // ИЗО
    SCULPTING = 1,      // Лепка
    DESIGN = 2,         // Дизайн
    TECHDRAWING = 3,    // Черчение
    SPECIALCOURSE = 4,  // Спецкурс
};
struct Lesson_Type_Prices
{
    static const size_t COUNT = 5;
    Price drawing;
    Price sculpting;
    Price design;
    Price techdrawing;
    Price specialcourse;
    Price get(Lesson_Type lesson_type)
    {
        switch (lesson_type)
        {
        case Lesson_Type::DRAWING: return drawing;
        case Lesson_Type::SCULPTING: return sculpting;
        case Lesson_Type::DESIGN: return design;
        case Lesson_Type::TECHDRAWING: return techdrawing;
        case Lesson_Type::SPECIALCOURSE: return specialcourse;
        default: IM_ASSERT(false);
        }
    }
}; // Defining it here so it's easier to sync with prices.h

class Attendance_Internal_Lesson
{
public:
    using Type = Lesson_Type;
private:
    struct Aday_Data
    {
        std::optional<Position<Teacher>> teacher;
        std::string comment;
    };
    const Type m_lesson_type;
    JTime m_begin;
    JTime m_end;
    std::vector<Aday_Data> m_aday_data;
    Vector_Sortable<Attendance_Student> m_attendance_students;
public:
    Attendance_Internal_Lesson(size_t holders_count, Type lesson_type, JTime begin, JTime end)
    : m_lesson_type(lesson_type), m_begin(begin), m_end(end), m_aday_data(holders_count)
    { }
    void set_time(JTime begin, JTime end)
    {
        m_begin = begin;
        m_end = end;
    }
    JTime get_time_begin() const { return m_begin; }
    JTime get_time_end() const { return m_end; }
    void set_teacher_pos(Aday aday, Position<Teacher> teacher_pos)
    {
        m_aday_data[aday.index()].teacher = teacher_pos;
    }
    void reset_teacher_pos(Aday aday)
    {
        m_aday_data[aday.index()].teacher.reset();
    }
    std::optional<Position<Teacher>> get_teacher_pos(Aday aday) const
    {
        return m_aday_data[aday.index()].teacher;
    }
    void set_comment(Aday aday, std::string comment)
    {
        m_aday_data[aday.index()].comment = comment;
    }
    std::string get_comment(Aday aday) const
    {
        return m_aday_data[aday.index()].comment;
    }
    Type get_lesson_type() const
    {
        return m_lesson_type;
    }
    void add_student(Position<Student> student_pos)
    {
        m_attendance_students.push_back(Ptr<Attendance_Student>::make(m_aday_data.size(), student_pos));
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