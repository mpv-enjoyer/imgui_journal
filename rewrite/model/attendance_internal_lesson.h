#pragma once
#include "attendance_student.h"
#include "teachers.h"
#include "lesson_infos.h"
#include <optional>

class AInternal_Lesson
{
public:
    using Type = Lesson_Infos::Type;
private:
    struct Aday_Data
    {
        std::optional<Pos<Teacher>> teacher;
        std::string comment;
    };
    const Type m_lesson_type;
    JTime m_begin;
    JTime m_end;
    std::vector<Aday_Data> m_aday_data;
    Vector_Sortable<AStudent> m_attendance_students;
public:
    AInternal_Lesson(size_t holders_count, Type lesson_type, JTime begin, JTime end)
    : m_lesson_type(lesson_type), m_begin(begin), m_end(end), m_aday_data(holders_count)
    { }
    void set_time(JTime begin, JTime end)
    {
        m_begin = begin;
        m_end = end;
    }
    JTime get_time_begin() const { return m_begin; }
    JTime get_time_end() const { return m_end; }
    void set_teacher_pos(Aday aday, Pos<Teacher> teacher_pos)
    {
        m_aday_data[aday.index()].teacher = teacher_pos;
    }
    void reset_teacher_pos(Aday aday)
    {
        m_aday_data[aday.index()].teacher.reset();
    }
    std::optional<Pos<Teacher>> get_teacher_pos(Aday aday) const
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
    // For Attendance_Merged_Lesson. Don't use elsewhere:
    void add_student(Pos<Student> student_pos)
    {
        m_attendance_students.push_back(Ptr<AStudent>::make(m_aday_data.size(), student_pos));
    }
    const AStudent& cref_student(Pos<AStudent> pos) const
    {
        return m_attendance_students.cref(pos);
    }
    AStudent& ref_student(Pos<AStudent> pos)
    {
        return m_attendance_students.ref(pos);
    }
    const Vector_Sortable<AStudent>& cref_students() const
    {
        return m_attendance_students;
    }
    Vector_Sortable<AStudent>& ref_data()
    {
        return m_attendance_students;
    }
};