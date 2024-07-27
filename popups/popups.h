#pragma once
#include "../graphical/graphical.h"

class Popup
{
private:
    bool accept_edit = false;
    std::string last_error = "";
    bool any_item_active = false;
    int popup_count = 1;
    bool popup_count_became_one = false;
protected:
    Graphical* graphical = nullptr;
    Journal* journal = nullptr;
    bool begin_frame(const char* label);
    bool should_exit();
public:
    bool check_ok() { return accept_edit; }
    void cancel() { accept_edit = false; }
    void ok() { accept_edit = true; }
    void error(std::string desc) { last_error = "ошибка: " + desc; }
    void print_error() { ImGui::TextColored({0.9f, 0.1f, 0.1f, 1.0f}, last_error.c_str()); }
};

#include "add_merged_lesson_to_journal.h"
#include "add_student_to_base.h"
#include "add_student_to_group.h"
#include "add_working_out.h"
#include "select_day_of_the_week.h"
#include "edit_lesson.h"
#include "confirm_delete_workout.h"
#include "confirm_delete_student.h"