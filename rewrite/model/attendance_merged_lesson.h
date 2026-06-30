#pragma once
#include "attendance_internal_lesson.h"

class Attendance_Merged_Lesson : public Removal_Info_Per_Month
{
public:
    static constexpr int AGE_GROUP_COUNT = 9;
    static std::array<std::string, AGE_GROUP_COUNT> get_all_age_groups()
    {
        const static std::array<std::string, AGE_GROUP_COUNT> AGE_GROUPS = 
            {"возраст не указан",
            "4 года, дошкольная группа",
            "5 лет, дошкольная группа",
            "6 лет, дошкольная группа",
            "7 лет, школьная группа",
            "8 лет, школьная группа",
            "9 лет, школьная группа",
            "10-11 лет, школьная группа",
            "12-13 лет, школьная группа"};
        return AGE_GROUPS;
    }
private:
    int m_number;
    int m_age_group;
    std::string m_comment;
    Vector_Sortable<Attendance_Internal_Lesson> m_internal_lessons;
    std::vector<Removal_Info_Per_Month> m_students_removal_info;
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
    void add_student(Position<Student> student_pos)
    {
        for (auto iter = m_internal_lessons.begin(); iter; iter.next())
        {
            iter->add_student(student_pos);
        }
        m_students_removal_info.emplace_back();
    }
    bool is_student_removed(Position<Attendance_Student> student_pos, Month month) const
    {
        return m_students_removal_info[student_pos.get()].is_removed(month);
    }
    void remove_student(Position<Attendance_Student> student_pos, Month month)
    {
        m_students_removal_info[student_pos.get()].remove(month);
    }
    void restore_student(Position<Attendance_Student> student_pos, Month month)
    {
        m_students_removal_info[student_pos.get()].restore(month);
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
    std::string get_group_description() const
    {
        std::stringstream output;
        output << "#" << std::to_string(get_number());
        output << ", " << get_all_age_groups()[m_age_group];
        if (m_comment.size() != 0) output << ", " << get_comment();
        return output.str();
    }

    std::string get_description() const
    {
        std::stringstream output;
        output << "Группа " << get_group_description();
        for (const auto& internal_lesson : cref_internal_lessons())
        {
            output << ", " << Lesson_Infos::get_name(internal_lesson.get_lesson_type()) << " ";
            output << internal_lesson.get_time_begin().to_string();
            output << " - ";
            output << internal_lesson.get_time_end().to_string();
        }
        return output.str();
    }
    std::string get_description(Position<Attendance_Internal_Lesson> pos)
    {
        std::stringstream output;
        output << "Группа " << get_group_description();
        const auto& internal_lesson = cref_internal_lessons()[pos];
        output << ", " << Lesson_Infos::get_name(internal_lesson.get_lesson_type()) << " ";
        output << internal_lesson.get_time_begin().to_string();
        output << " - ";
        output << internal_lesson.get_time_end().to_string();
        return output.str();
    }
    std::string get_age_group_string() const
    {
        return get_all_age_groups()[m_age_group];
    }

    AUTOOPS1(Attendance_Merged_Lesson, cref_internal_lessons().cbegin()->get_time_begin());
};