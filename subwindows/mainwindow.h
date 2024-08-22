#pragma once
#include "subwindows.h"

class Mainwindow : public Subwindow
{
    void table(int merged_lesson_id);
    bool table_row(int merged_lesson_id, int internal_student_id, int counter);
    int table_cell(int merged_lesson_id, int internal_student_id, int visible_day_id);
    bool attendance_combo(const char* label, int* status, std::string tooltip);
    void table_add_student_row(int merged_lesson_id, int counter);
    void table_add_workout_row(int merged_lesson_id, int counter, std::vector<std::vector<int>>* attended_counter_increase = nullptr);
    void table_student_count_row(int merged_lesson_id, std::vector<std::vector<int>> attended_counter_increase);
public:
    enum Callback
    {
        none,
        month_left,
        month_default,
        month_right
    };
    Callback get_callback();

    Mainwindow(Graphical* graphical);
    void show_frame();
private:
    Callback _callback = none;
};


