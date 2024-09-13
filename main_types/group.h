#pragma once
#include "main_types.h"

#define ATTEND_BOTH ((Group::AttendData)0)
#define ATTEND_FIRST ((Group::AttendData)1)
#define ATTEND_SECOND ((Group::AttendData)2)

template<class Archive>
void serialize(Archive & ar, Students_List & g, const unsigned int version)
{
    ar & g.attend_data;
    ar & g.student;
    ar & g.is_deleted;
}

class Group
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & number;
        ar & age_group;
        ar & _students;
        ar & comment;
    }
public:
    NO_IMPLICIT_CONVERSION_T_CHECKED(short, AttendData, value < MAX_INTERNAL_LESSONS * MAX_INTERNAL_LESSONS);
    NO_IMPLICIT_CONVERSION_T(int, Number);
    NO_IMPLICIT_CONVERSION_T_CHECKED(std::size_t, AgeGroup, value < AGE_GROUP_COUNT);
    NO_IMPLICIT_CONVERSION_T(std::string, Comment);
    NO_IMPLICIT_CONVERSION_T(std::size_t, StudentID);
    NON_COPYABLE_NOR_MOVABLE(Group);
private:
    class Students_List : public Removable
    {
    public:
        Student* student;
        AttendData attend_data;
    };
    Group() { };
    Number _number = -1;
    AgeGroup _age_group = 0;
    std::vector<Students_List> _students;
    Comment _comment = Comment("");
public:
    Group(Number number, AgeGroup age_group);
    StudentID get_size() const;
    const Student &get_student(StudentID id) const;
    Number get_number() const;
    bool set_number(Number number);
    StudentID find_student(const Student& student) const;
    AgeGroup get_age_group() const;
    bool set_age_group(AgeGroup age_group);
    bool should_attend(StudentID id, InternalLessonID internal_lesson_id) const;
    AttendData get_attend_data(StudentID id) const;
    bool set_attend_data(StudentID id, AttendData new_attend_data);
    StudentID add_student(Student& new_student);
    bool delete_student(StudentID id);
    bool is_in_group(const Student &student, StudentID *id) const;
    bool is_deleted(StudentID id) const;
    bool restore_student(StudentID id);
    bool set_comment(Comment comment);
    Comment get_comment() const;
    std::string get_description() const;
    time_t get_removed_timestamp(StudentID id) const;
    bool operator==(const Group &rhs) const;
};