#pragma once
#include "workout.h"

// TODO: an ability to workout with unset real lesson
class Workouts
{
    NON_COPYABLE_NOR_MOVABLE(Workouts);
    std::vector<Workout> m_workouts;
public:
    Workouts() { }
    void add(Workout workout)
    {
        if (std::find(m_workouts.begin(), m_workouts.end(), workout) == m_workouts.end())
        {
            m_workouts.push_back(workout);
        }
    }
    void remove(Workout workout)
    {
        auto it = std::find(m_workouts.begin(), m_workouts.end(), workout);
        if (it != m_workouts.end())
        {
            m_workouts.erase(it);
        }
    }
    std::vector<Workout> get_by_real(Internal_Lesson_ID internal_lesson_id, Aday aday) const
    {
        std::vector<Workout> output;
        for (auto workout : m_workouts)
        {
            if (workout.real_aday() == aday && workout.real_internal_lesson_id() == internal_lesson_id)
            {
                output.push_back(workout);
            }
        }
        return output;
    }
    std::optional<Workout> is_should(Attendance_ID attendance_id) const
    {
        for (auto workout : m_workouts)
        {
            if (workout.should_id() == attendance_id) return workout;
        }
        return {};
    }
    std::optional<Workout> is_real(Internal_Lesson_ID internal_lesson_id, Aday aday, Internal_Student_ID should_student_id) const
    {
        for (const auto& workout : m_workouts)
        {
            if (workout.real_aday() != aday) continue;
            if (workout.real_internal_lesson_id() != internal_lesson_id) continue;
            if (workout.should_id().internal_student_id() != should_student_id) continue;
            return workout;
        }
        return {};
    }
};