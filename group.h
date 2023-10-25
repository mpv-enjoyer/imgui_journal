#pragma once
#include "main.h"
#include "helper_types.h"
#include "student.h"

struct Students_List
{
    Student* student;
    bool is_deleted;
    Attend_Data attend_data; //this is like student_ignore_lesson_ids
};

class Group
{
private:
    Group_Pair group_info;
    std::vector<Students_List> students;
    std::string comment;
public:
    Group();
    int get_size() const;
    int get_number() const; bool set_number(int new_number);
    int find_student(Student& student) const;
    int get_day_of_the_week() const;
    bool set_day_of_the_week(int new_day);
    Attend_Data get_attend_data(int known_student_id) const;
    bool set_attend_data(int known_student_id, Attend_Data new_attend_data);
    Student& get_student(int student) const; int add_student(Student& new_student); bool delete_student(Student& to_remove_student);
    std::string get_comment(); bool set_comment(std::string new_comment);
    std::string get_description();
    bool is_in_group(Student& student) const;
    bool operator==(const Group& rhs) const;
};
