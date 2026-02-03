#pragma once
#include "attendance_internal_lesson.h"

class IMerged_Lesson : public Removal_Info
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
    IMerged_Lesson(std::vector<Ptr<AInternal_Lesson>> internal_lessons, int number, int age_group, std::string comment)
    { }
    std::vector<Aday> adays(Month month) const; // decorate as {aday} in Workout_Merged_Lesson, calc in Attendance_Merged_Lesson
    const Vector_Sortable<AInternal_Lesson>& cref_internal_lessons() const;
    Vector_Sortable<AInternal_Lesson>& ref_internal_lessons();
    void add_student(Pos<Student> student_pos); // decorate as { return; } in Workout_Merged_Lesson
    bool is_student_removed(Pos<AStudent> student_pos) const;
    void remove_student(Pos<AStudent> student_pos);
    void restore_student(Pos<AStudent> student_pos);
    std::vector<Pos<Student>> get_student_positions();
    int get_age_group() const;
    void set_age_group(int age_group);
    int get_number() const;
    void set_number(int number);
    std::string get_comment() const;
    void set_comment(std::string comment);
};