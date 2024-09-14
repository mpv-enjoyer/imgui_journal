#include "calendar_day.h"

void Calendar_Day::Attendance::sync()
{
    for (std::size_t i = 0; i < merged_lesson_attendance.size(); i++)
    {
        merged_lesson_attendance[i].sync();
    }
    for (std::size_t i = merged_lesson_attendance.size(); i < lessons->lesson_infos().size(); i++)
    {
        merged_lesson_attendance.push_back(MergedLessonAttendance(lessons->lesson_info(i)));
    }
}

StudentAttendance Calendar_Day::Attendance::get(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id) const
{
    return merged_lesson_attendance[merged_lesson_id].get(internal_lesson_id, student_id);
}

TeacherName Calendar_Day::Attendance::get(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id) const
{
    return merged_lesson_attendance[merged_lesson_id].get(internal_lesson_id);
}

void Calendar_Day::Attendance::set(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, TeacherName teacher_name)
{
    merged_lesson_attendance[merged_lesson_id].set(internal_lesson_id, teacher_name);
}

void Calendar_Day::Attendance::set(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance attendance)
{
    merged_lesson_attendance[merged_lesson_id].set(internal_lesson_id, student_id, attendance);
}

Calendar_Day::Attendance::Attendance(Lessons_Day *lessons) : lessons(lessons)
{
    for (std::size_t i = 0; i < lessons->lesson_infos().size(); i++)
    {
        merged_lesson_attendance.push_back(MergedLessonAttendance(lessons->lesson_info(i)));
    }
};

void Calendar_Day::Attendance::MergedLessonAttendance::sync()
{
    for (std::size_t i = 0; i < internal_lesson_attendance.size(); i++)
    {
        internal_lesson_attendance[i].sync();
    }
}

StudentAttendance Calendar_Day::Attendance::MergedLessonAttendance::get(InternalLessonID internal_lesson_id, Group::StudentID student_id) const
{
    return internal_lesson_attendance[internal_lesson_id].get(student_id);
}

TeacherName Calendar_Day::Attendance::MergedLessonAttendance::get(InternalLessonID internal_lesson_id) const
{
    return internal_lesson_attendance[internal_lesson_id].get();
}

void Calendar_Day::Attendance::MergedLessonAttendance::set(InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance attendance)
{
    internal_lesson_attendance[internal_lesson_id].set(student_id, attendance);
}

void Calendar_Day::Attendance::MergedLessonAttendance::set(InternalLessonID internal_lesson_id, TeacherName teacher_name)
{
    internal_lesson_attendance[internal_lesson_id].set(teacher_name);
}

Calendar_Day::Attendance::MergedLessonAttendance::MergedLessonAttendance(const Lesson_Info *lesson_info) : lesson_info(lesson_info)
{
    for (std::size_t i = 0; i < lesson_info->get_lessons_size(); i++)
    {
        internal_lesson_attendance.push_back(InternalLessonAttendance(&(lesson_info->get_group())));
    }
}

void Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::sync()
{
    for (std::size_t i = student_attendance.size(); i < group->get_size(); i++)
    {
        student_attendance.push_back(StudentAttendance());
    }
}

TeacherName Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::get() const
{
    return teacher_name;
}

StudentAttendance Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::get(Group::StudentID student_id) const
{
    return student_attendance[student_id];
}

void Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::set(Group::StudentID student_id, StudentAttendance attendance)
{
    student_attendance[student_id] = attendance;
}

void Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::set(TeacherName teacher_name)
{
    this->teacher_name = teacher_name;
}

Calendar_Day::Attendance::MergedLessonAttendance::InternalLessonAttendance::InternalLessonAttendance(const Group *group) : group(group)
{
    student_attendance = std::vector<StudentAttendance>(group->get_size().value);
};

Calendar_Day::Calendar_Day(Lessons_Day *lessons_day) : lessons_day(lessons_day), attendance(lessons_day) { }

MergedLessonID Calendar_Day::find_merged_lesson(const Lesson_Info& lesson_info) const
{
    const auto lesson_infos = lessons_day->lesson_infos();
    for (MergedLessonID i = 0; i.value < lesson_infos.size(); i.value++)
    {
        if (lesson_infos[i] == &lesson_info) return i;
    }
    return -1;
}

bool Calendar_Day::set_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance student_attendance)
{
    attendance.set(merged_lesson_id, internal_lesson_id, student_id, student_attendance);
    return true;
}

StudentAttendance Calendar_Day::get_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id)
{
    return attendance.get(merged_lesson_id, internal_lesson_id, student_id);
}

TeacherName Calendar_Day::get_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id) const
{
    return attendance.get(merged_lesson_id, internal_lesson_id);
}

void Calendar_Day::set_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, TeacherName teacher_name)
{
    attendance.set(merged_lesson_id, internal_lesson_id, teacher_name);
}

void Calendar_Day::student_added()
{
    sync();
}

void Calendar_Day::lesson_info_added()
{
    sync();
}

void Calendar_Day::sync()
{
    attendance.sync();
}