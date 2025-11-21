#pragma once
#include "attendance_internal_lesson.h"

class Attendance_Merged_Lesson : public Removal_Info
{
public:
    static constexpr int AGE_GROUP_COUNT = 9;
    std::array<std::string, AGE_GROUP_COUNT> AGE_GROUPS = 
        {"возраст не указан",
        "4 года, дошкольная группа",
        "5 лет, дошкольная группа",
        "6 лет, дошкольная группа",
        "7 лет, школьная группа",
        "8 лет, школьная группа",
        "9 лет, школьная группа",
        "10-11 лет, школьная группа",
        "12-13 лет, школьная группа"};
    struct Aday_With_Status
    {
        Aday aday;
        bool is_active;
    };
private:
    int m_number;
    int m_age_group;
    std::string m_comment;
    Vector_Sortable<Attendance_Internal_Lesson> m_internal_lessons;
    std::vector<bool> m_students_removal_info;
    std::vector<Aday_With_Status> m_adays;
public:
    Attendance_Merged_Lesson(std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons, int number, int age_group, std::string comment, std::vector<Aday_With_Status> adays)
    : m_number(number), m_age_group(age_group), m_comment(comment), m_adays(adays)
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
    void add_student(Position<Student> student_pos)
    {
        for (auto iter = m_internal_lessons.begin(); iter; iter.next())
        {
            iter->add_student(student_pos);
        }
        m_students_removal_info.push_back(false);
    }
    bool is_student_removed(Position<Attendance_Student> student_pos) const
    {
        return m_students_removal_info[student_pos.get()];
    }
    void remove_student(Position<Attendance_Student> student_pos)
    {
        m_students_removal_info[student_pos.get()] = true;
    }
    void restore_student(Position<Attendance_Student> student_pos)
    {
        m_students_removal_info[student_pos.get()] = false;
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
    void set_active_adays(std::vector<bool> adays)
    {
        DEBUG_ASSERT(adays.size() == m_adays.size());
        for (size_t i = 0; i < adays.size(); i++)
        {
            m_adays[i].is_active = adays[i];
        }
    }
    std::vector<Aday_With_Status> get_adays() const
    {
        return m_adays;
    }
};

using Aday_With_Status = Attendance_Merged_Lesson::Aday_With_Status;