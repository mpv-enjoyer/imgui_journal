#include "save.h"

template<class Archive>
void serialize(Archive& ar, Student& g, const unsigned int version)
{
    ar & g.get_age_group();
    ar & g.
}

void test_save()
{
    Student student;
    student.set_name("name 1");
    student.set_contract(0);
    student.set_age_group(3);
}