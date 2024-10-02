#include "calendar_day.h"

Calendar_Day::Calendar_Day(Lessons_Day *lessons_day) : _lessons_day(lessons_day), _attendance(lessons_day) { }

const Lessons_Day *Calendar_Day::lessons_day() const
{
    return _lessons_day;
}

bool Calendar_Day::set_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance student_attendance)
{
    _attendance.set_status(merged_lesson_id, internal_lesson_id, student_id, student_attendance);
    return true;
}

StudentAttendance Calendar_Day::get_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id) const
{
    return _attendance.get(merged_lesson_id, internal_lesson_id, student_id);
}

Calendar_Day::TeacherName Calendar_Day::get_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id) const
{
    return _attendance.get(merged_lesson_id, internal_lesson_id).teacher_name;
}

void Calendar_Day::set_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, TeacherName teacher_name)
{
    _attendance.set_teacher_name(merged_lesson_id, internal_lesson_id, teacher_name);
}

void Calendar_Day::student_added()
{
    _attendance.sync();
}

void Calendar_Day::lesson_info_added()
{
    _attendance.sync();
}

Calendar_Day::Attendance::Attendance(const Lessons_Day *lessons_day) : _lessons_day(lessons_day)
{
    sync();
}

void Calendar_Day::Attendance::sync()
{
    for (MergedLessonID merged_lesson_id = _data.size(); merged_lesson_id < _lessons_day->size(); merged_lesson_id.value++)
    {
        _data.emplace_back(_lessons_day->get(merged_lesson_id)->size());
    }
}

Calendar_Day::InternalAttendance Calendar_Day::Attendance::get(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id) const
{
    return _data[merged_lesson_id][internal_lesson_id];
}

StudentAttendance Calendar_Day::Attendance::get(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id) const
{
    return _data[merged_lesson_id][internal_lesson_id].student_attendance[student_id];
}

void Calendar_Day::Attendance::set_teacher_name(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, TeacherName teacher_name)
{
    _data[merged_lesson_id][internal_lesson_id].teacher_name = teacher_name;
}

void Calendar_Day::Attendance::set_status(MergedLessonID merged_lesson_id, InternalLessonID internal_lesson_id, Group::StudentID student_id, StudentAttendance attendance)
{
    _data[merged_lesson_id][internal_lesson_id].student_attendance[student_id] = attendance;
}
