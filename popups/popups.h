#pragma once
#include "../graphical/graphical.h"

class Popup
{
private:
    bool accept_edit = false;
    std::string last_error = "";
protected:
    Graphical* graphical = nullptr;
    Journal* journal = nullptr;
public:
    bool check_ok() { return accept_edit; }
    void cancel() { accept_edit = false; }
    void ok() { accept_edit = true; }
    void error(std::string desc) { last_error = "ошибка: " + desc; }
    void print_error() { ImGui::TextDisabled(last_error.c_str()); }
    void sync_data(Graphical* _graphical, Journal* _journal)
    {
        graphical = _graphical;
        journal = _journal;
    }
};

#include "add_merged_lesson_to_journal.h"
#include "add_student_to_base.h"
#include "add_student_to_group.h"
#include "add_working_out.h"
#include "select_day_of_the_week.h"
#include "edit_lesson.h"
#include "confirm.h"