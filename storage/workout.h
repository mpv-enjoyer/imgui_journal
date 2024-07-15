#pragma once
#include "../main_types/main_types.h"
#include "../helpers/helpers.h"

class Workout_Handler
{
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << _bottom_year;
        ar << _top_year;
        ar << _all_workouts;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> _bottom_year;
        ar >> _top_year;
        std::vector<Workout_Info_> workouts;
        ar >> workouts;
        _all_workouts.clear();
        _real_hashes.clear();
        _should_hashes.clear();
        _last_real_hashes.clear();
        for (auto workout : workouts)
        {
            insert_info(workout);
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    int _bottom_year;
    int _top_year;
    std::vector<Workout_Info_> _all_workouts;
    std::unordered_set<Workout_Hash_Container, Real_Workout_Hash> _real_hashes;
    std::unordered_set<Workout_Hash_Container, Last_Real_Workout_Hash> _last_real_hashes;
    std::unordered_set<Workout_Hash_Container, Should_Workout_Hash> _should_hashes;
    bool remove_student(int month, int student_id);
    // we don't need this if students never get deleted.
public:
    Workout_Handler(int month, int year);
    Workout_Handler();
    static int get_bottom_year(int month, int year);
    int bottom_year();
    int top_year();
    bool is_month_here(int month, int year);
    int get_year(int month);
    void insert_info(Workout_Info_ workout_info);
    std::vector<std::vector<const Workout_Info_ *>> get_info(int real_month, int real_wday, Lesson real_lesson);
    const Workout_Info_* get_info(int should_month, int should_mday, Lesson lesson, int should_student_id);
    const std::vector<const Workout_Info_*> search_info(int should_month);
    void delete_info(const Workout_Info_ *workout_info);
    bool change_lesson_info_position(int month, int wday, int old_merged_lesson_id, int new_merged_lesson_id, int max_merged_lessons_size);
    // set old_merged_lesson_id to -1 if new lesson.
    // set new_merged_lesson_id to -1 if removed a lesson.
    // returns false if tried to delete a valid lesson_info.
};
