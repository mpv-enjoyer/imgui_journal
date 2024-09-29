#pragma once
#include "subwindows.h"
#include <array>

class Subwindow_Students_List : public Subwindow
{
    struct Lesson_Info_Position
    {
        int wday;
        int merged_lesson;
        int internal_student_id;
    };
    std::vector<std::pair<std::vector<Lesson_Info_Position>, int>> lessons_per_student;
    const ImVec4 background = ImVec4(0.7f, 0.85f, 0.85f, 1.0f);
    const ImVec4 background_filtered = ImVec4(0.5f, 0.75f, 0.65f, 1.0f);
    ImGuiTextFilter text_filter;
public:
    Subwindow_Students_List(Graphical* graphical);
    void update_lessons_per_student();
    void update_lessons_per_student(int student_id, int index);
    void append_students_to_begin();
    bool show_frame();
};