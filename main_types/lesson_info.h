#pragma once
#include "main_types.h"

template<class Archive>
void serialize(Archive & ar, JTime & g, const unsigned int version)
{
    ar & g.hours;
    ar & g.minutes;
}

template<class Archive>
void serialize(Archive & ar, InternalLessonInfo & g, const unsigned int version)
{
    ar & g.lesson_name_id;
    ar & g.time_begin;
    ar & g.time_end;
}

class InternalLessonInfos
{
    InternalLessonInfos() { };
    std::vector<InternalLessonInfo> _lesson_pairs;
public:
    InternalLessonInfos(std::vector<InternalLessonInfo> lesson_pairs) : _lesson_pairs(lesson_pairs) { };
    std::vector<InternalLessonInfo> get_lesson_pairs() const { return _lesson_pairs; };
    InternalLessonInfo get_lesson_pair(InternalLessonID id) const { IM_ASSERT(id.value < size()); };
    const std::size_t size() const { return _lesson_pairs.size(); };
    void edit_pair(InternalLessonID id, JTime begin, JTime end) { IM_ASSERT(id.value < size()); _lesson_pairs[id].time_begin = begin; _lesson_pairs[id].time_end = end; };
};

class Lesson_Info : public Removable //can contain multiple lessons which will be merged in the table.
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & group;
        ar & lesson_pairs;
        ar & removed;
    }
private:
    Lesson_Info() : _internal_lesson_infos({(InternalLessonInfo){.lesson_name_id = -1, .time_begin = {0}, .time_end = {0}}}), _group(nullptr) {};
    Group* const _group;
    InternalLessonInfos _internal_lesson_infos;
public:
    Lesson_Info(Group& connected_group, std::vector<InternalLessonInfo> internal_lesson_infos);
    const Group& get_group() const;
    Group& get_group();
    std::vector<InternalLessonInfo> get_lesson_pairs() const;
    InternalLessonInfo get_lesson_pair(InternalLessonID id) const;
    void edit_lesson_pair(InternalLessonID id, JTime begin, JTime end);
    bool should_attend(int known_internal_student_id, int internal_lesson) const = delete; /* Check in group */
    InternalLessonID get_lessons_size() const;
    std::string get_description(int current_internal_lesson = -1) const;
    std::string get_name(int internal_student_id) const;
    bool operator==(const Lesson_Info& rhs) const;
    bool operator!=(const Lesson_Info& rhs) const;
    bool operator< (const Lesson_Info& rhs) const;
    bool operator> (const Lesson_Info& rhs) const;
};