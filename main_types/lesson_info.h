#pragma once
#include "main_types.h"

template<class Archive>
void serialize(Archive & ar, JTime & g, const unsigned int version)
{
    ar & g.hours;
    ar & g.minutes;
}

template<class Archive>
void serialize(Archive & ar, Lesson_Pair & g, const unsigned int version)
{
    ar & g.lesson_name_id;
    ar & g.time_begin;
    ar & g.time_end;
}

class Lesson_Info //can contain multiple lessons which will be merged in the table.
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
    bool removed = 0;
    Group* group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info() {};
    Lesson_Info(Group& connected_group);
    const Group& get_group() const;
    Group& _group();
    bool set_group(Group &new_group);
    Lesson_Pair get_lesson_pair(int id) const; bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    bool discontinue(); bool is_discontinued() const; bool restore();
    bool should_attend(int known_internal_student_id, int internal_lesson) const;
    int get_lessons_size() const;
    std::string get_description(int current_internal_lesson = -1) const;
    std::string get_name(int internal_student_id) const;
    bool operator==(const Lesson_Info& rhs) const;
    bool operator!=(const Lesson_Info& rhs) const;
    bool operator< (const Lesson_Info& rhs) const;
    bool operator> (const Lesson_Info& rhs) const;
};