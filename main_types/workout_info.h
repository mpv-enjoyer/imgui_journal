#pragma once
#include "main_types.h"
#include <unordered_set>
#include <boost/container_hash/hash.hpp>

struct Workout_Info_
{
    int student_id;
    Lesson_Info* real_lesson_info; //nullptr if unavailable
    int real_internal_lesson;
    std::tm real_attend;
    Lesson_Info* should_lesson_info; //nullptr if unavailable
    int should_internal_lesson;
    std::tm should_attend;
};

struct Workout_Hash_Container
{
    const Workout_Info_* info;
    bool operator==(const Workout_Hash_Container &other) const
    {
        return (info->student_id             == other.info->student_id
            &&  info->real_attend            == other.info->real_attend
            &&  info->should_attend          == other.info->should_attend
            &&  info->real_lesson_info       == other.info->real_lesson_info
            &&  info->should_lesson_info     == other.info->should_lesson_info
            &&  info->real_internal_lesson   == other.info->real_internal_lesson
            &&  info->should_internal_lesson == other.info->should_internal_lesson);
    }
};

struct Real_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info->real_attend.tm_mon);
        boost::hash_combine(seed, s.info->real_attend.tm_wday);
        boost::hash_combine(seed, s.info->real_lesson_info);
        boost::hash_combine(seed, s.info->real_internal_lesson);
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
        boost::hash_combine(seed, s.info->real_lesson_info);
        boost::hash_combine(seed, s.info->real_internal_lesson);
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
        boost::hash_combine(seed, s.info->should_lesson_info);
        boost::hash_combine(seed, s.info->should_internal_lesson);
        return seed;
    }
};