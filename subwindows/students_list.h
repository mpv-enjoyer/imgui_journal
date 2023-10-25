#pragma once
#include "subwindows.h"

class Subwindow_Students_List : public Subwindow
{
private:
    std::vector<Student*>& all_students;
    std::vector<Group*>& all_groups;
    Popup_Add_Student_To_Base* popup_add_student_to_base = nullptr;
public:
    Subwindow_Students_List(std::vector<Student*>& all_students, std::vector<Group*>& all_groups) : all_students(all_students), all_groups(all_groups) { };
    bool show_frame();
};