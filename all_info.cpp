#include "main.h"
#include "helpers/helpers.h"
#include "main_types/main_types.h"
//months start from September [0] up to august.

struct Predefined_Lesson
{
    std::tm date;
    Lesson_Info* clone_from;
};

struct Month_Info
{
    std::vector<Calendar_Day*>& all_days;
    std::vector<std::vector<Lesson_Info*>>& all_lessons;
    std::vector<Group*>& all_groups;
    std::vector<Student*>& all_students;
};

class All_Info
{
    std::vector<std::vector<Calendar_Day*>> days; // [month][day]
    std::vector<std::vector<std::vector<Lesson_Info*>>> lessons; // [month][day_of_the_week][merged_lesson]
    std::vector<std::vector<Group*>> groups; // [month][group]
    std::vector<std::vector<Student*>> students; // [month][student]
    std::vector<std::tm> holidays;
    std::vector<std::vector<Predefined_Lesson>> predefined_lessons;
    int current_month;
    int current_year; // 2023 - 2024 years => 2023 - 1900
    std::string canonic_year_name();
    std::string canonic_month_name();
    void generate_month(int month);
public:
    All_Info()
    {
        days = std::vector<std::vector<Calendar_Day*>>(12);
        lessons = std::vector<std::vector<std::vector<Lesson_Info*>>>(12);
        groups = std::vector<std::vector<Group*>>(12);
        students = std::vector<std::vector<Student*>>(12);
        predefined_lessons = std::vector<std::vector<Predefined_Lesson>>(12);
    };
    void set_current_month(int month, int year)
    {
        current_month = (month + 4) % 12;
        current_year = year;
        if (current_month < 8) current_year--;
        load();
    };
    Month_Info get_current_month();
    void load()
    {
        for (int month = 0; month < 12; month++)
        {
            std::string input_file_name = canonic_year_name() + "/" + canonic_month_name();
            auto input = std::ifstream(input_file_name);
            if (input.fail()) 
            {
                generate_month(month);
            }
            else
            {
                boost::archive::text_iarchive input_archive(input);
                input_archive >> days;
                input_archive >> lessons;
                input_archive >> groups;
                input_archive >> students;
                input_archive >> predefined_lessons;
            }
        }
        
    };
    void save_current_year();
};