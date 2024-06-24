#pragma once
#include "../main_types/main_types.h"
#include "../helpers/helpers.h"

class Workout_Handler
{
    int _bottom_year;
    int _top_year;
    std::vector<Workout_Info_> _all_workouts;
    std::unordered_set<Workout_Hash_Container, Real_Workout_Hash> _real_hashes;
    std::unordered_set<Workout_Hash_Container, Last_Real_Workout_Hash> _last_real_hashes;
    std::unordered_set<Workout_Hash_Container, Should_Workout_Hash> _should_hashes;
    void insert_info(Workout_Info_ workout_info);
    std::vector<std::vector<const Workout_Info_*>> get_info(int real_month, int real_wday, Lesson_Info* real_lesson_info, int internal_lesson_info);
    void delete_info(const Workout_Info_* workout_info);
};
