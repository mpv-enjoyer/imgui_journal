#pragma once
#include "main_types.h"

struct Students_List
{
    Student* student;
    bool is_deleted;
    Attend_Data attend_data; //this is like student_ignore_lesson_ids
};


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
        ar & students;
        ar & comment;
    }
private:
    int number;
    int age_group;
    std::vector<Students_List> students;
    std::string comment;
public:
    Group();
    int get_size() const;
    int get_number() const; bool set_number(int new_number);
    int find_student(const Student& student) const;
    int get_age_group() const;
    bool set_age_group(int new_day);
    bool check_with_attend_data(int known_student_id, int internal_lesson) const;
    bool check_no_attend_data(const Student &student) const;
    Attend_Data get_attend_data(int known_student_id) const;
    bool set_attend_data(int known_student_id, Attend_Data new_attend_data);
    const Student& get_student(int student) const; int add_student(Student& new_student); bool delete_student(Student& to_remove_student);
    std::string get_comment() const; bool set_comment(std::string new_comment);
    std::string get_description() const;
    bool is_in_group(const Student& student) const;
    bool is_deleted(const Student & student) const;
    bool restore_student(Student &student);
    bool operator==(const Group &rhs) const;
};
