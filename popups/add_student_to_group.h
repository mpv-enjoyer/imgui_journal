#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Add_Student_To_Group : public Popup
{
private:
    int current_group_id = -1;
    int merged_lesson_known_id;
    int current_selected_student = -1;
    int current_wday = -1;
    ImGuiTextFilter text_filter;
    const Group& current_group;
    const Lesson_Info& current_lesson;
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
public:
    Popup_Add_Student_To_Group(Graphical* graphical, const Lesson_Info& current_lesson, int merged_lesson_known_id, int wday);
    int get_merged_lesson_known_id() { IM_ASSERT(check_ok()); return merged_lesson_known_id; };
    const Student* get_added_student() { IM_ASSERT(check_ok()); return journal->student(current_selected_student); };
    const Group* get_current_group() { IM_ASSERT(check_ok()); return &current_group; };
    bool show_frame();
    bool is_ok_possible(bool select_visible) 
    { 
        if (!select_visible || current_selected_student == -1) error("Выберите ученика");
        return select_visible && current_selected_student!=-1;
    }
    void accept_changes();
};