#pragma once
#include "icommand.h"

class Add_Workout : public ICommand
{
    Workout_Lesson_ID m_real_pos;
    Workout_Lesson_ID m_should_pos;
    Position<Student> m_student_pos;
    static Error get_lesson_id_error(const IModel& model, Workout_Lesson_ID lesson_id, Position<Student> student_pos, bool should_be_here)
    {
        auto wday = Wday::make_from_mday(lesson_id.mday);
        auto& merged_lessons = model->attendance_wdays()->cref_wday(wday).cref_merged_lessons();
        if (!merged_lessons.is_pos_valid(lesson_id.merged_lesson_pos))
        {
            return "invalid merged_lesson_pos";
        }
        auto& internal_lessons = merged_lessons.cref(lesson_id.merged_lesson_pos).cref_internal_lessons();
        if (!internal_lessons.is_pos_valid(lesson_id.internal_lesson_pos))
        {
            return "invalid internal_lesson_pos";
        }
        auto& lesson_students = merged_lessons.cref(lesson_id.merged_lesson_pos).cref_students();
        auto found = std::any_of(lesson_students.cbegin(), lesson_students.cend(),
            [student_pos](const Attendance_Student& current)
            {
                return current.get_student_pos() == student_pos && THIS IS WRONG: current.get_wants_lesson();
                This doesnt work for a second internal lesson. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            });
        if (found != should_be_here)
        {
            return "found != should_be_here";
        }
        return {};
    }

    static Error get_not_awaited_error(const IModel& model, Workout_Lesson_ID should_pos, Position<Student> student_pos)
    {
        auto& merged = model
            ->attendance_wdays()
            ->cref_wday(Wday::make_from_mday(should_pos.mday))
            .cref_merged_lessons()
            .cref(should_pos.merged_lesson_pos);
        auto it = std::find_if(merged.cref_students().begin(), merged.cref_students().end(),
            [student_pos](Attendance_Student& student)
            {
                return student.get_student_pos() == student_pos;
            });

        auto status = it->get_holder(should_pos.mday).get_status();
        switch (status)
        {
        case Attendance_Status::NOT_AWAITED: return "Ученик не должен приходить на этот урок";
        case Attendance_Status::ON_LESSON: return "Ученик присутствовал на этом уроке";
        default: return {};
        }
    }
    // if (select_student == -1) { error("Выберите ученика"); return false; }
    // if (!is_calendar_filled) { error("Для выбранного ученика нет доступных отработок"); return false; }
    // if (select_day == -1) { error("Выберите день"); return false; }
    // if (select_lesson == Lesson {-1, -1}) { error("Выберите урок"); return false; }
    // int student_id = select_student;
    // int wday = current_journal->wday(select_day);
    // const Lesson_Info& select_lesson_info = PTRREF(current_journal->lesson_info(wday, select_lesson.merged_lesson_id));
    // const Student& student = PTRREF(current_journal->student(student_id));
    // Student_Status requested_status = current_journal->day(select_day)->get_status(select_lesson_info, select_lesson.internal_lesson_id, student);
    // if (requested_status.status == STATUS_WORKED_OUT) { error("Отработка уже назначена"); return false; }
    // if (requested_status.status == STATUS_NOT_AWAITED) { error("Ученик не должен приходить на этот урок"); return false; }
    // if (requested_status.status == STATUS_ON_LESSON) { error("Ученик присутствовал на этом уроке"); return false; }
    // if (requested_status.status == STATUS_INVALID) { error("SYSTEM_STATUS_INVALID"); return false; }
    // return true;
public:
    Add_Workout(Workout_Lesson_ID real_pos, Workout_Lesson_ID should_pos, Position<Student> student_pos)
    : m_real_pos(real_pos), m_should_pos(should_pos), m_student_pos(student_pos)
    { }
    Error get_error(const IModel& model) override
    {
        if (!model->students()->cref_students().is_pos_valid(m_student_pos))
        {
            return "invalid student_pos";
        }
        auto& student = model->students()->cref_students().cref(m_student_pos);
        if (auto error = get_lesson_id_error(model, m_real_pos, m_student_pos, false))
        {
            return "real_pos: " + *error;
        }
        if (auto error = get_lesson_id_error(model, m_should_pos, m_student_pos, true))
        {
            return "should_pos: " + *error;
        }
        if (auto error = get_not_awaited_error(model, m_should_pos, m_student_pos))
        {
            return error;
        }
        auto& workouts = model->workouts();
        if (workouts->is_should(m_should_pos, m_student_pos) || workouts->is_real(m_real_pos, m_student_pos))
        {
            return "Отработка уже назначена";
        }
        if (student.cref_removal_info().is_removed())
        {
            return "Ученик удален";
        }
        if ()

    if (current_student->is_removed()) return false;
    if (select_lesson->is_discontinued()) return false;
    int internal_student_id = select_lesson->get_group().find_student(PTRREF(current_student));
    if (internal_student_id == -1) return false;
    if (!select_lesson->should_attend(internal_student_id, select_internal_lesson)) return false;
    const Lesson_Pair pair = select_lesson->get_lesson_pair(select_internal_lesson);
    int current_lesson_name_id = pair.lesson_name_id;
    if (_match_lesson_types(caller_lesson_name_id, current_lesson_name_id))
        return true;
    return false;

        return {};
    }

    void call(IModel& model) override
    {
        Workout_Lesson_ID real_pos
        {
            .mday = ,
            .merged_lesson_pos = ,
            .internal_lesson_pos = 
        };
        Workout workout()
        model->workouts()->add()
    }
};