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
public:
    void insert_info(Workout_Info_ workout_info);
    std::vector<std::vector<const Workout_Info_*>> get_info(int real_month, int real_wday, Lesson real_lesson);
    const Workout_Info_ *get_info(int should_month, std::tm should_day, Lesson lesson, const Student *student);
    void delete_info(const Workout_Info_ *workout_info);
    bool change_lesson_info_position(int month, int wday, int old_merged_lesson_id, int new_merged_lesson_id, bool is_new = false, bool is_remove = false, int max_merged_lessons_size = -1);
    // old_merged_lesson_id set to -1 if is_new = true.
    // returns false if tried to delete a valid lesson_info.
};
