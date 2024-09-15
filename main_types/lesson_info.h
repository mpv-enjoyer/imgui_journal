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

struct InternalLessonInfo
{
    NO_IMPLICIT_CONVERSION_T_CHECKED(std::size_t, ID, value < LESSON_TYPE_COUNT);
    JTime time_begin;
    JTime time_end;
    const ID lesson_name_id;
    InternalLessonInfo(ID lesson_name_id) : lesson_name_id(lesson_name_id) { };
};

class Lesson_Info : public Removable, public Container<InternalLessonInfo> //can contain multiple lessons which will be merged in the table.
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
    Lesson_Info() : _group(nullptr) {};
    Group* const _group;
    void set(InternalLessonInfo data, Container<InternalLessonInfo>::ID id) override;
public:
    Lesson_Info(Group& connected_group, std::vector<InternalLessonInfo> internal_lesson_infos);
    const Group& get_group() const;
    Group& get_group();
    std::vector<InternalLessonInfo> get_lesson_pairs() const = delete; /* use get() */
    InternalLessonInfo get_lesson_pair(ID id) const = delete; /* use get(id) */
    void edit_lesson_pair(ID id, JTime begin, JTime end);
    bool should_attend(int known_internal_student_id, int internal_lesson) const = delete; /* use group::should_attend() */
    ID get_lessons_size() const = delete; /* use size() */
    std::string get_description(ID id) const;
    std::string get_description() const;
    bool operator==(const Lesson_Info& rhs) const = delete;
    bool operator!=(const Lesson_Info& rhs) const = delete;
    bool operator< (const Lesson_Info& rhs) const;
    bool operator> (const Lesson_Info& rhs) const;
};

#define MAX_INTERNAL_LESSONS 2 /* legacy */
typedef Lesson_Info::ID InternalLessonID;
static const Lesson_Info::ID INTERNAL_LESSON_SIZE = 2;
static const InternalLessonID INTERNAL_LESSON_FIRST = 0;
static const InternalLessonID INTERNAL_LESSON_SECOND = 1;