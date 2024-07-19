#pragma once

#include "main_types.h"
#include <unordered_set>
#include <boost/container_hash/hash.hpp>

struct Workout_Info_
{
    int real_student_id;
    Lesson real_lesson;
    std::tm real_attend;
    int should_student_id; // The same as real_student_id except for when it's for the other month
    Lesson should_lesson;
    std::tm should_attend;
template<class Archive>
void save(Archive & ar, const unsigned int version) const
{
    ar << real_student_id;
    ar << real_lesson;
    ar << Time_Archiver(real_attend);
    ar << should_student_id;
    ar << should_lesson;
    ar << Time_Archiver(should_attend);
}

template<class Archive>
void load(Archive & ar, const unsigned int version)
{
    ar >> real_student_id;
    ar >> real_lesson;
    Time_Archiver real_attend_ar;
    ar >> real_attend_ar;
    real_attend = real_attend_ar.to_tm();
    ar >> should_student_id;
    ar >> should_lesson;
    Time_Archiver should_attend_ar;
    ar >> should_attend_ar;
    should_attend = should_attend_ar.to_tm();
}
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct Workout_Hash_Container
{
    const std::vector<Workout_Info_>* all_workouts;
    int index;
    const Workout_Info_ info() const { return all_workouts->at(index); };
    bool operator==(const Workout_Hash_Container &other) const
    {
        return (info().real_student_id   == other.info().real_student_id
            &&  info().real_lesson       == other.info().real_lesson
            &&  info().real_attend       == other.info().real_attend
            &&  info().should_student_id == other.info().should_student_id
            &&  info().should_attend     == other.info().should_attend
            &&  info().should_lesson     == other.info().should_lesson);
    }
};

struct Real_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info().real_attend.tm_mon);
        boost::hash_combine(seed, s.info().real_attend.tm_wday);
        boost::hash_combine(seed, s.info().real_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info().real_lesson.internal_lesson_id);
        return seed;
    }
};

struct Last_Real_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info().real_student_id);
        boost::hash_combine(seed, s.info().real_attend.tm_mon);
        boost::hash_combine(seed, s.info().real_attend.tm_wday);
        boost::hash_combine(seed, s.info().real_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info().real_lesson.internal_lesson_id);
        return seed;
    }
};

struct Should_Workout_Hash
{
    std::size_t operator()(const Workout_Hash_Container& s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.info().should_attend.tm_mon);
        boost::hash_combine(seed, s.info().should_attend.tm_mday);
        boost::hash_combine(seed, s.info().should_lesson.merged_lesson_id);
        boost::hash_combine(seed, s.info().should_lesson.internal_lesson_id);
        boost::hash_combine(seed, s.info().should_student_id);
        return seed;
    }
};