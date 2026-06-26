#pragma once
#include "attendance_holder.h"
#include "common/removal_info.h"
#include "student.h"

class AStudent
{
    NON_COPYABLE(AStudent);
    const Pos<Student> m_student_pos;
    std::vector<Ptr<AHolder>> m_holders;
    // Use this student in an internal lesson for a discount + visual student in group
    // Change this through set_wants_lesson, remove_student, remove_lesson
    std::vector<bool> m_wants_lessons_by_month;
public:
    AStudent(std::size_t holders_count, Pos<Student> student_pos)
    : m_student_pos(student_pos), m_wants_lessons_by_month(Month::COUNT, false)
    {
        for (std::size_t i = 0; i < holders_count; i++)
        {
            m_holders.push_back(Ptr<AHolder>::make());
        }
    }
    bool get_wants_lesson(Month month) const
    {
        return m_wants_lessons_by_month[month.calculate_study_year_index()];
    }
    void set_wants_lesson(Month month, bool value)
    {
        m_wants_lessons_by_month[month.calculate_study_year_index()] = value;
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