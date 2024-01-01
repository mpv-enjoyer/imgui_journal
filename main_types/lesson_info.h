#pragma once
#include "main_types.h"

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    bool removed = 0;
    Group& group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info(Group& connected_group);
    Group& get_group() const; bool set_group(Group& new_group);
    Lesson_Pair get_lesson_pair(int id) const; bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    bool discontinue(); bool is_discontinued();
    bool should_attend(Student& student) const;
    int get_lessons_size() const;
    std::string get_description(int current_internal_lesson = -1) const;
    bool operator==(const Lesson_Info& rhs) const;
    bool operator!=(const Lesson_Info& rhs) const;
    bool operator< (const Lesson_Info& rhs) const;
    bool operator> (const Lesson_Info& rhs) const;
    void save(boost::archive::text_oarchive& ar, const unsigned int version) const;
    void load(boost::archive::text_iarchive& ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};