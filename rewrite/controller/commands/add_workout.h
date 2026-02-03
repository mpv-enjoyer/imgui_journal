#pragma once
#include "icommand.h"

class Add_Workout : public ICommand
{
    Workout m_workout;

    static Error get_error_not_awaited(AStatus status)
    {
        switch (status)
        {
        case AStatus::NOT_AWAITED: return "Ученик не должен приходить на этот урок";
        case AStatus::ON_LESSON: return "Ученик присутствовал на этом уроке";
        default: return {};
        }
    }

    static Error get_error_removed_merged_lesson(const IModel& model, Merged_Lesson_ID id)
    {
        if (model->cref_merged_lesson(id).is_removed()) return "Группа удалена";
        return {};
    }

    static Error get_error_removed_student(const IModel& model, Internal_Student_ID should_id)
    {
        auto& attendance_student = model->cref_attendance_student(should_id);
        Pos<Student> student_pos = attendance_student.get_student_pos();
        if (model->students()->cref_students().cref(student_pos).is_removed()) return "Ученик удален";
        if (!attendance_student.get_wants_lesson()) return "Ученик удален из группы";
        return {};
    }

    static Error get_error_student_in_real_lesson(const IModel& model, Internal_Student_ID should_id, Internal_Lesson_ID real_id)
    {
        auto student_pos = model->cref_attendance_student(should_id).get_student_pos();
        auto student_positions_in_real = model->cref_merged_lesson(real_id).get_student_positions();
        for (auto current : student_positions_in_real)
        {
            if (current == student_pos && model->cref_attendance_student(should_id).get_wants_lesson()) return "Ученик уже есть в этой группе";
        }
        return {};
    }

    static Error get_error_already_worked_out(const Ptr<Workouts>& workouts, const Workout& workout)
    {
        if (workouts->is_should(workout.should_id())) return "Отработка уже назначена";
        if (workouts->is_real(workout.real_internal_lesson_id(), workout.real_aday(), workout.should_id()))
        {
            return "Отработка уже назначена";
        }
        return {};
    }

    static Error get_error_lessons_type_dont_match(Lesson_Type real, Lesson_Type should)
    {
        if (real == should) return {};
        if (real == Lesson_Type::DESIGN         && should == Lesson_Type::SPECIALCOURSE) return {};
        if (real == Lesson_Type::SPECIALCOURSE  && should == Lesson_Type::DESIGN) return {};
        if (real == Lesson_Type::DESIGN         && should == Lesson_Type::DRAWING) return {};
        if (real == Lesson_Type::DRAWING        && should == Lesson_Type::DESIGN) return {};
        if (real == Lesson_Type::DRAWING        && should == Lesson_Type::SPECIALCOURSE) return {};
        if (real == Lesson_Type::SPECIALCOURSE  && should == Lesson_Type::DRAWING) return {};
        return "Неверное сопоставление should и real для workout.";
    }
public:
    Add_Workout(Workout workout)
    : m_workout(workout)
    { }
    Error get_error(const IModel& model) const override
    {
        Error e = {};
        false
         || (e = get_error_removed_merged_lesson(model, m_workout.real_internal_lesson_id()))
         || (e = get_error_removed_merged_lesson(model, m_workout.should_id()))
         || (e = get_error_removed_student(model, m_workout.should_id()))
         || (e = get_error_student_in_real_lesson(model, m_workout.should_id(), m_workout.real_internal_lesson_id()))
         || (e = get_error_already_worked_out(model->workouts(), m_workout))
         || (e = get_error_not_awaited(model->cref_attendance_student(m_workout.should_id()).cref_holder(m_workout.should_id().aday()).get_status()))
         || (e = get_error_lessons_type_dont_match(model->cref_internal_lesson(m_workout.real_internal_lesson_id()).get_lesson_type(), model->cref_internal_lesson(m_workout.should_id()).get_lesson_type()));
        return e;
    }

    void call(IModel& model) override
    {
        model->workouts()->add(m_workout);
    }
};