#pragma once
#include "main_types.h"
#include <unordered_set>
#include <boost/container_hash/hash.hpp>

struct Workout_Info_
{
    int student_id;
    Lesson real_lesson;
    std::tm real_attend;
    Lesson should_lesson;
    std::tm should_attend;

    bool operator==(const Workout_Info_ &other) const
    {
        return (student_id == other.student_id
            && real_attend == other.real_attend
            && should_attend == other.should_attend
            && real_lesson == other.real_lesson
            && should_lesson == other.should_lesson);
    }
};

struct Workout_Hash_Container
{
    Workout_Info_* info;
};

struct Real_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info->real_attend.tm_mon);
        boost::hash_combine(seed, s.info->real_attend.tm_wday);
        boost::hash_combine(seed, s.info->real_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info->real_lesson.internal_lesson_id);
        return seed;
    }
};

struct Last_Real_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info->student_id);
        boost::hash_combine(seed, s.info->real_attend.tm_mon);
        boost::hash_combine(seed, s.info->real_attend.tm_wday);
        boost::hash_combine(seed, s.info->real_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info->real_lesson.internal_lesson_id);
        return seed;
    }
};

struct Should_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info->should_attend.tm_mon);
        boost::hash_combine(seed, s.info->should_attend.tm_mday);
        boost::hash_combine(seed, s.info->should_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info->should_lesson.internal_lesson_id);
        return seed;
    }
};