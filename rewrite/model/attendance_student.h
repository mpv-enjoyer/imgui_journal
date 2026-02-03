#pragma once
#include "attendance_holder.h"
#include "common/removal_info.h"
#include "student.h"

class AStudent
{
    NON_COPYABLE(AStudent);
    const Pos<Student> m_student_pos;
    bool m_wants_lesson = true;
    std::vector<Ptr<AHolder>> m_holders;
public:
    AStudent(std::size_t holders_count, Pos<Student> student_pos)
    : m_student_pos(student_pos)
    {
        for (std::size_t i = 0; i < holders_count; i++)
        {
            m_holders.push_back(Ptr<AHolder>::make());
        }
    }
    bool get_wants_lesson() const
    {
        return m_wants_lesson;
    }
    void set_wants_lesson(bool value)
    {
        m_wants_lesson = value;
    }
    const AHolder& cref_holder(Aday aday) const
    {
        return *(m_holders[aday.index()]);
    }
    AHolder& ref_holder(Aday aday)
    {
        return *(m_holders[aday.index()]);
    }
    Pos<Student> get_student_pos() const
    {
        return m_student_pos;
    }
};