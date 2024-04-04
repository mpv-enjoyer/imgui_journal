#pragma once
#include "subwindows.h"

class Subwindow_Students_List : public Subwindow
{
    struct Lesson_Info_Position
    {
        int wday;
        int merged_lesson;
        int internal_student_id;
    };
    std::vector<std::vector<Lesson_Info_Position>> lessons_per_student; 
public:
    Subwindow_Students_List(Graphical* graphical);
    void update_lessons_per_student();
    void update_lessons_per_student(int student_id);
    bool show_frame();
};