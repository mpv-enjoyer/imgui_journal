#include "main.h"
#include "helpers/helpers.h"
//months start from September [0] up to august.

struct Predefined_Lesson
{
    std::tm date;
    Lesson_Info* clone_from;
};

class All_Info
{
    std::vector<std::vector<Calendar_Day*>> days; // [month][day]
    std::vector<std::vector<std::vector<Lesson_Info*>>> lessons; // [month][day_of_the_week][merged_lesson]
    std::vector<std::vector<Group*>> groups; // [month][group]
    std::vector<std::vector<Student*>> students; // [month][student]
    std::vector<std::tm> holidays; //TODO

};