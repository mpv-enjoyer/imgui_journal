#pragma once
#include "popups.h"

class Popup_Add_Student_To_Group : public Popup
{
private:
    int current_group_id = -1;
    int merged_lesson_known_id;
    int current_selected_student = -1;
    ImGuiTextFilter text_filter;
    std::vector<Student*>& all_students;
    Group& current_group;
    Lesson_Info& current_lesson;
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
public:
    Popup_Add_Student_To_Group(Lesson_Info& current_lesson, std::vector<Student*>& all_students, int merged_lesson_known_id);
    int get_merged_lesson_known_id() { IM_ASSERT(check_ok()); return merged_lesson_known_id; };
    Student* get_added_student() { IM_ASSERT(check_ok()); return all_students[current_selected_student]; };
    Group* get_current_group() { IM_ASSERT(check_ok()); return &current_group; };
    bool show_frame();
    bool is_ok_possible(bool select_visible) 
    { 
        if (!select_visible || current_selected_student!=-1) error("Выберите ученика");
        return select_visible && current_selected_student!=-1; }
    void accept_changes(std::vector<Visible_Day>& visible_days);
};