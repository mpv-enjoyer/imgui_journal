#pragma once
#include "workout.h"

class Workouts
{
    std::vector<Ptr<Workout>> m_workouts;
public:
    Workouts() { }
    void add(Workout* workout)
    {
        for (const auto& current_workout : m_workouts)
        {
            if (*workout == *current_workout) return;
        }
        m_workouts.emplace_back(workout);        
    }
    std::vector<Workout> get_by_real(Workout::Position pos) const
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
    bool is_should(Workout::Position workout_pos, Vector_Sortable<Student>::Position student_pos) const
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
    bool is_real(Workout::Position workout_pos, Vector_Sortable<Student>::Position student_pos) const
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