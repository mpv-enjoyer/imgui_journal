#pragma once
#include "attendance_internal_lesson.h"

class Attendance_Merged_Lesson : public Removal_Info
{
public:
    static constexpr int AGE_GROUP_COUNT = 8;
    std::array<std::string, AGE_GROUP_COUNT> AGE_GROUPS = 
        {"возраст не указан"
        "4 года, дошкольная группа", 
        "5 лет, дошкольная группа", 
        "6 лет, дошкольная группа", 
        "7 лет, школьная группа", 
        "8 лет, школьная группа", 
        "9 лет, школьная группа", 
        "10-11 лет, школьная группа", 
        "12-13 лет, школьная группа"};
private:
    int m_number;
    int m_age_group;
    std::string m_comment;
    Vector_Sortable<Attendance_Internal_Lesson> m_internal_lessons;
public:
    Attendance_Merged_Lesson(std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons, int number, int age_group, std::string comment)
    : m_number(number), m_age_group(age_group), m_comment(comment)
    {
        IM_ASSERT(internal_lessons.size() != 0);
        for (auto& internal_lesson : internal_lessons)
        {
            m_internal_lessons.push_back(std::move(internal_lesson));
        }
    }
    const Vector_Sortable<Attendance_Internal_Lesson>& cref_internal_lessons() const
    {
        return m_internal_lessons;
    }
    Vector_Sortable<Attendance_Internal_Lesson>& ref_internal_lessons()
    {
        return m_internal_lessons;
    }
    void add_student(std::size_t holders_count, Position<Student> student_pos)
    {
        for (auto iter = m_internal_lessons.begin(); iter; iter.next())
        {
            iter->add_student(holders_count, student_pos);
        }
    }

    // Cache this maybe?
    std::vector<Position<Student>> get_student_positions() const
    {
        std::vector<Position<Student>> positions;
        for (auto& attendance_student : m_internal_lessons.cbegin()->cref_students())
        {
            positions.push_back(attendance_student.get_student_pos());
        }
        return positions;
    }
    
    // bad idea. students contain unique info.
    //const Vector_Sortable<Attendance_Student>& cref_students() const
    //{
    //    return m_internal_lessons.cbegin()->cref_data();
    //}
    //Vector_Sortable<Attendance_Student>& ref_students()
    //{
    //    return m_internal_lessons.begin()->ref_data();
    //}
    int get_age_group() const
    {
        return m_age_group;
    }
    void set_age_group(int age_group)
    {
        m_age_group = age_group;
    }
    int get_number() const
    {
        return m_number;
    }
    void set_number(int number)
    {
        m_number = number;
    }
    std::string get_comment() const
    {
        return m_comment;
    }
    void set_comment(std::string comment)
    {
        m_comment = comment;
    }
};