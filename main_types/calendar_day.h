#pragma once
#include "main_types.h"

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
    std::vector<Workout_Info> workouts;
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

NO_IMPLICIT_CONVERSION_T(int, AttendanceStatus);
NO_IMPLICIT_CONVERSION_T(int, DiscountStatus);
struct StudentAttendance
{
    AttendanceStatus status = STATUS_NO_DATA;
    DiscountStatus discount_status = -1;
    StudentAttendance() {};
};
NO_IMPLICIT_CONVERSION_T(std::string, TeacherName);

class Calendar_Day
{
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & lessons;
        ar & attendance_info;
        ar & teacher_names;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & lessons;
        ar & attendance_info;
        if (version > 0)
        {
            ar & teacher_names;
        }
        else
        {
            teacher_names = std::vector<std::vector<std::string>>(lessons->size(), std::vector<std::string>(2));
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    NON_COPYABLE_NOR_MOVABLE(Calendar_Day);
    Calendar_Day() { };
    class InternalLessonAttendance : public Syncable<Group, StudentAttendance, StudentAttendance> { };
    class MergedLessonAttendance : public Syncable<Lesson_Info, InternalLessonAttendance, StudentAttendance, Group::ID> { };
    class Attendance : public Syncable<Lessons_Day, MergedLessonAttendance, StudentAttendance, InternalLessonID, Group::ID> { };
    Lessons_Day* const lessons_day = nullptr;
    Attendance attendance;
    void sync();
public:
    Calendar_Day(Lessons_Day* lessons_day);
    MergedLessonID find_merged_lesson(const Lesson_Info& lesson_info) const;
    bool set_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance student_attendance);
    StudentAttendance get_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id);
    TeacherName get_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id) const;
    void set_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, TeacherName teacher_name);
    /* Now syncing functions */
    void student_added();
    void lesson_info_added();
};

BOOST_CLASS_VERSION(Calendar_Day, 1); // Version 1: added teacher_names

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