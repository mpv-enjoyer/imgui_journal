#include "serializable.h"

std::vector<Student*> _all_students;
std::vector<Group*> _all_groups;
std::vector<std::vector<Lesson_Info*>> _all_lessons(7, std::vector<Lesson_Info*>()); //[day_of_the_week][]
std::vector<Calendar_Day*> _all_days;

std::vector<Student *> &_all_students_()
{
    return std::ref(_all_students);
}

std::vector<Group *> &_all_groups_()
{
    return std::ref(_all_groups);
}

std::vector<std::vector<Lesson_Info *>> &_all_lessons_()
{
    return std::ref(_all_lessons);
}

std::vector<Calendar_Day *> &_all_days_()
{
    return std::ref(_all_days);
}
