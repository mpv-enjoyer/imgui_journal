#include "internal.h"

bool Frame_Data::increment_internal_lesson()
{
    _internal_lesson++;
    int max = Journal::lesson_info(Graphical::wday, merged_lesson)->get_lessons_size();
    if (_internal_lesson >= max) 
    {
        _internal_lesson = 0;
        return false;
    }
    else return true;
}

bool Frame_Data::increment_internal_student_id()
{
    _internal_student_id++;
    int max = Journal::lesson_info(Graphical::wday, merged_lesson)->get_group().get_size();
    if (_internal_student_id >= max) 
    {
        _internal_student_id = 0;
        return false;
    }
    else return true;
}

bool Frame_Data::increment_merged_lesson()
{
    _merged_lesson++;
    int max = Journal::lesson_info_count(Graphical::wday);
    if (_merged_lesson >= max)
    {
        _merged_lesson = 0;
        return false;
    }
    else return true;
}

bool Frame_Data::increment_visible_day_id()
{
    _visible_day_id++;
    int max = Graphical::visible_days.size();
    if (_visible_day_id >= max)
    {
        _visible_day_id = 0;
        return false;
    }
    else return true;
}

bool Frame_Data::increment_visible_student_counter()
{
    _visible_student_counter++;
    return true;
}