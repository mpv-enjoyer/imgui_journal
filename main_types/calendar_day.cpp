#include "calendar_day.h"

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
    attendance.set(student_attendance, merged_lesson_id, internal_lesson_id, student_id);
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