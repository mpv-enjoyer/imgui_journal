#pragma once
#include "icommand.h"

class Add_Workout : public ICommand
{
    Workout_Lesson_ID m_workout;

    static Error get_error_not_awaited(Attendance_Status status)
    {
        switch (status)
        {
        case Attendance_Status::NOT_AWAITED: return "Ученик не должен приходить на этот урок";
        case Attendance_Status::ON_LESSON: return "Ученик присутствовал на этом уроке";
        default: return {};
        }
    }

    static Error get_error_removed_merged_lesson(const Ptr<Attendance_Wdays>& wdays, Merged_Lesson_ID id)
    {
        if (wdays->cref_merged_lesson(id).cref_removal_info().is_removed()) return "Группа удалена";
        return {};
    }

    static Error get_error_removed_student(const IModel& model, Internal_Student_ID should_id)
    {
        auto& attendance_student = model->attendance_wdays()->cref_attendance_student(should_id);
        Position<Student> student_pos = attendance_student.get_student_pos();
        if (model->students()->cref_students().cref(student_pos).cref_removal_info().is_removed()) return "Ученик удален";
        if (!attendance_student.get_wants_lesson()) return "Ученик удален из группы";
        return {};
    }

    static Error get_error_student_in_real_lesson(const Ptr<Attendance_Wdays>& wdays, Internal_Student_ID should_id, Internal_Lesson_ID real_id)
    {
        auto student_pos = wdays->cref_attendance_student(should_id).get_student_pos();
        auto student_positions_in_real = wdays->cref_merged_lesson(real_id.merged_lesson_id()).get_student_positions();
        for (auto current : student_positions_in_real)
        {
            if (current == student_pos && wdays->cref_attendance_student(should_id).get_wants_lesson()) return "Ученик уже есть в этой группе";
        }
        return {};
    }

    static Error get_error_already_worked_out(const Ptr<Workouts>& workouts, const Workout_Lesson_ID& workout)
    {
        if (workouts->is_should(workout.should_id())) return "Отработка уже назначена";
        if (workouts->is_real(workout.real_internal_lesson_id(), workout.real_aday(), workout.should_id().internal_student_id()))
        {
            return "Отработка уже назначена";
        }
        return {};
    }

    static Error get_error_lessons_type_dont_match(Lesson_Type real, Lesson_Type should)
    {
        using T = Lesson_Type;
        if (real == should) return {};
        if (real == T::DESIGN && should == T::SPECIALCOURSE) return {};
        if (real == T::SPECIALCOURSE && should == T::DESIGN) return {};
        if (real == T::DESIGN && should == T::DRAWING) return {};
        if (real == T::DRAWING && should == T::DESIGN) return {};
        if (real == T::DRAWING && should == T::SPECIALCOURSE) return {};
        if (real == T::SPECIALCOURSE && should == T::DRAWING) return {};
        return "Неверное сопоставление should и real для workout.";
    }
public:
    Add_Workout(Workout_Lesson_ID workout)
    : m_workout(workout)
    { }
    Error get_error(const IModel& model) override
    {
        auto& wdays = model->attendance_wdays();
        Error e = {};
        false
         || (e = get_error_removed_merged_lesson(wdays, m_workout.real_internal_lesson_id().merged_lesson_id()))
         || (e = get_error_removed_merged_lesson(wdays, m_workout.should_id().merged_lesson_id()))
         || (e = get_error_removed_student(model, m_workout.should_id().internal_student_id()))
         || (e = get_error_student_in_real_lesson(wdays, m_workout.should_id().internal_student_id(), m_workout.real_internal_lesson_id()))
         || (e = get_error_already_worked_out(model->workouts(), m_workout))
         || (e = get_error_not_awaited(wdays->cref_attendance_student(m_workout.should_id().internal_student_id()).get_holder(m_workout.should_id().aday()).get_status()))
         || (e = get_error_lessons_type_dont_match(wdays->cref_internal_lesson(m_workout.real_internal_lesson_id()).get_lesson_name(), wdays->cref_internal_lesson(m_workout.should_id().internal_lesson_id()).get_lesson_name()));
        return e;
    }

    void call(IModel& model) override
    {
        model->workouts()->add(m_workout);
    }
};