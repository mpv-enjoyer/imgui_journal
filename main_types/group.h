#pragma once
#include "main_types.h"

#define ATTEND_BOTH ((AttendData)0)
#define ATTEND_FIRST ((AttendData)1)
#define ATTEND_SECOND ((AttendData)2)
#define ATTEND_SIZE ((AttendData)3)

NO_IMPLICIT_CONVERSION_T_CHECKED(short, AttendData, value < ATTEND_SIZE);
class StudentAttendData : public Removable
{
public:
    Student* const student;
    AttendData attend_data;
};

class Group : public Container<StudentAttendData>
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
    NO_IMPLICIT_CONVERSION_T(int, Number);
    NO_IMPLICIT_CONVERSION_T_CHECKED(std::size_t, AgeGroup, value < AGE_GROUP_COUNT);
    NO_IMPLICIT_CONVERSION_T(std::string, Comment);
    typedef ID StudentID;
    NO_IMPLICIT_CONVERSION_T(std::size_t, SortedStudentID);
    NON_COPYABLE_NOR_MOVABLE(Group);
private:
    Group() { };
    Number _number = -1;
    AgeGroup _age_group = 0;
    class StudentsSortMap
    {
        StudentsSortMap() { };
        const Group* _group;
        std::vector<StudentID> _map;
    public:
        StudentsSortMap(Group* group) : _group(group) { sync(); }
        void sync();
        StudentID get(SortedStudentID id) { IM_ASSERT(id.value < _map.size()); return _map[id]; };
    };
    StudentsSortMap _students_sort_map = StudentsSortMap(this);
    Comment _comment = Comment("");
    void set(StudentAttendData data, ID id) override;
public:
    Group(Number number, AgeGroup age_group);
    StudentID get_size() const = delete; // use size()
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
    bool is_in_group(const Student &student, StudentID *id = nullptr) const;
    Removable::IsRemoved is_deleted(StudentID id) const;
    bool restore_student(StudentID id);
    bool set_comment(Comment comment);
    Comment get_comment() const;
    std::string get_description() const;
    time_t get_removed_timestamp(StudentID id) const;
    bool operator==(const Group &rhs) const;
    /* Now syncing functions */
    void student_changed(Student* student);
};