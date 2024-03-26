#pragma once
#include "main_types.h"

struct Time_Archiver
{
    int year;
    int month;
    int mday;
    int wday;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
    ar & mday;
    ar & month;
    ar & year;
    ar & wday;
}
};

struct Workout_Info
{
    const Student* student = nullptr;
    Lesson_Info* lesson_info = nullptr;
    int internal_lesson;
    std::tm date;
    Lesson recovery_hint = {-1, -1};
    Workout_Info() {};
    Workout_Info(const Student* _student, Lesson_Info* _lesson_info, int _internal_lesson, std::tm _date) : student(_student), lesson_info(_lesson_info), internal_lesson(_internal_lesson), date(_date) {};
template<class Archive>
void save(Archive & ar, const unsigned int version) const
{
    ar << student;
    ar << lesson_info;
    ar << internal_lesson;
    Time_Archiver arch;
    arch.mday = date.tm_mday;
    arch.month = date.tm_mon;
    arch.year = date.tm_year;
    arch.wday = date.tm_wday;
    ar << arch;
    ar << recovery_hint;
}

template<class Archive>
void load(Archive & ar, const unsigned int version)
{
    ar >> student;
    ar >> lesson_info;
    ar >> internal_lesson;
    Time_Archiver arch;
    ar >> arch;
    date.tm_mday = arch.mday;
    date.tm_mon = arch.month;
    date.tm_year = arch.year;
    date.tm_wday = arch.wday;
    ar >> recovery_hint;
}
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct Student_Status
{
    int status;
    Workout_Info workout_info;
    int discount_status = -1;
    Student_Status() {};
};

struct Internal_Attendance_Status
{
    std::vector<Student_Status> planned;
    std::vector<Workout_Info>  workouts;
    Internal_Attendance_Status() {};
};

template<class Archive>
void serialize(Archive & ar, Lesson & g, const unsigned int version)
{
    ar & g.merged_lesson_id;
    ar & g.internal_lesson_id;
}

template<class Archive>
void serialize(Archive & ar, Student_Status & g, const unsigned int version)
{
    ar & g.status;
    ar & g.discount_status;
    ar & g.workout_info;
}

template<class Archive>
void serialize(Archive & ar, Internal_Attendance_Status & g, const unsigned int version)
{
    ar & g.planned;
    ar & g.workouts;
}

class Calendar_Day
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & lessons;
        ar & attendance_info;
    }
private:
    std::vector<Lesson_Info*>* lessons;
    std::vector<std::vector<Internal_Attendance_Status>> attendance_info;
public:
    Calendar_Day() { };
    Calendar_Day(std::vector<Lesson_Info*>& lessons_in_this_day); //LESSONS MUST BE SORTED BEFORE CALLING.
    bool set_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int status);
    bool set_status(Lesson known_lesson, int known_id_student, int status);
    bool insert_workout_into_status(Lesson known_lesson, int known_id_student, Workout_Info workout_info);
    Student_Status get_status(const Lesson_Info &merged_lesson, int internal_lesson, const Student &student) const;
    Student_Status get_status(Lesson known_lesson, int known_id_student) const;
    bool add_workout(Student& student_to_workout, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to);
    bool add_workout(int known_id_student, Lesson known_lesson_from, Lesson_Info& merged_to, int internal_to, std::tm cached_time_to);
    bool add_workout(Student& student_to_workout, Lesson known_lesson_from, Workout_Info new_workout_info);
    int get_workout_size(Lesson_Info& merged_lesson, int internal_lesson) const;
    int get_workout_size(Lesson known_lesson) const;
    const Student& get_workout_student(Lesson_Info& merged_lesson, int internal_lesson, int known_workout_id) const;
    const Student* get_workout_student(Lesson known_lesson, int known_workout_id) const;
    Workout_Info get_workout_info(Lesson known_lesson, int known_workout_id) const;
    Workout_Info get_workout_info(Lesson known_lesson, const Student& student) const;
    bool delete_workout(Lesson_Info& merged_lesson, int internal_lesson, const Student& student);
    bool delete_workout(Lesson known_lesson, int known_workout_id);
    bool set_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student, int discount_status);
    bool set_discount_status(Lesson known_lesson, int internal_lesson, Student& student, int discount_status);
    bool set_discount_status(Lesson known_lesson, int known_student_id, int discount_status);
    int get_discount_status(Lesson_Info& merged_lesson, int internal_lesson, Student& student);
    int get_discount_status(Lesson known_lesson, int known_id_student);
    int find_merged_lesson(const Lesson_Info& l_info) const;
    int find_student(Student& student, int known_merged_lesson_id) const;
    //the following is needed to properly update the journal
    bool add_student_to_group(Group& group, Student& new_student, int known_new_student_id);
    bool add_student_to_group(int known_merged_lesson_id, Student& new_student, int known_new_student_id);
    bool swap_merged_lessons(int old_id, int new_id);
    bool add_merged_lesson(Lesson_Info& new_lesson_info, bool await_no_one, int known_new_merged_lesson_id);
    //...
};

struct _Day_With_Info
{
    int number; //WARNING: THIS STARTS WITH 1
    Calendar_Day* day;
    bool is_future;
    bool is_today;
};

struct Day_With_Info
{
    int number; //WARNING: THIS STARTS WITH 1
    const Calendar_Day* day;
    bool is_future;
    bool is_today;
};