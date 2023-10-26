#pragma once
#include "../main.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"

class Popup
{
private:
    bool accept_edit = false;
    std::string last_error = "";
public:
    bool check_ok() { return accept_edit; }
    void cancel() { accept_edit = false; }
    void ok() { accept_edit = true; }
    void error(std::string desc) { last_error = "ошибка: " + desc; }
    void print_error() { ImGui::TextDisabled(last_error.c_str()); }
};

#include "add_merged_lesson_to_journal.h"
#include "add_student_to_base.h"
#include "add_student_to_group.h"
#include "add_working_out.h"
#include "select_day_of_the_week.h"