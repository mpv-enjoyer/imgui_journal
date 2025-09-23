#pragma once
#include "workout.h"

class Workouts
{
    std::vector<Ptr<Workout>> m_workouts;
public:
    Workouts() { }
    void add(Ptr<Workout> workout)
    {
        for (const auto& current_workout : m_workouts)
        {
            if (*workout == *current_workout) return;
        }
        m_workouts.push_back(std::move(workout));        
    }
    std::vector<Workout> get_by_real(Workout_Lesson_ID pos) const
    {
        std::vector<Workout> output;
        for (const auto& workout : m_workouts)
        {
            if (workout->get_real_pos() == pos)
            {
                output.push_back(*workout);
            }
        }
        return output;
    }
    bool is_should(Workout_Lesson_ID workout_pos, Vector_Sortable<Student>::Position student_pos) const
    {
        std::vector<Workout> output;
        for (const auto& workout : m_workouts)
        {
            if (workout->get_should_pos() == workout_pos && workout->get_student_pos() == student_pos)
            {
                return true;
            }
        }
        return false;
    }
    bool is_real(Workout_Lesson_ID workout_pos, Vector_Sortable<Student>::Position student_pos) const
    {
        std::vector<Workout> output;
        for (const auto& workout : m_workouts)
        {
            if (workout->get_real_pos() == workout_pos && workout->get_student_pos() == student_pos)
            {
                return true;
            }
        }
        return false;
    }
};