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
    NO_IMPLICIT_CONVERSION_T(std::size_t, SortedStudentID);
    NON_COPYABLE_NOR_MOVABLE(Group);
private:
    class StudentAttendData : public Removable
    {
    public:
        Student* const student;
        AttendData attend_data;
    };
    class StudentsList
    {
        std::vector<StudentAttendData> _students;
    public:
        StudentsList() { };
        void add(Student* student) { _students.push_back((StudentAttendData){.student = student, .attend_data = ATTEND_BOTH}); };
        const std::vector<StudentAttendData> get() const { return _students; };
        StudentAttendData get(StudentID id) const { IM_ASSERT(id < size()); return _students[id]; };
        void set(StudentID id, AttendData attend_data) { IM_ASSERT(id < size()); _students[id].attend_data = attend_data; };
        bool set(StudentID id, Removable::IsRemoved is_removed) { IM_ASSERT(id < size()); if (is_removed) { _students[id].remove(); } else { _students[id].restore(); } };
        StudentID size() const { return _students.size(); };
    };
    Group() { };
    Number _number = -1;
    AgeGroup _age_group = 0;
    StudentsList _students;
    class StudentsSortMap
    {
        StudentsSortMap() { };
        StudentsList* _students_list;
        std::vector<StudentID> _map;
    public:
        StudentsSortMap(StudentsList* students_list) : _students_list(students_list) { sync(); }
        void sync();
        StudentID get(SortedStudentID id) { IM_ASSERT(id.value < _map.size()); return _map[id]; };
    };
    StudentsSortMap _students_sort_map = StudentsSortMap(&_students);
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