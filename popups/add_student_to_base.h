#pragma once
#include "popups.h"
#include "../helpers/helpers.h"
#include "../main_types/main_types.h"
#include "../storage/journal.h"

class Popup_Add_Student_To_Base : public Popup
{
private:
    std::string name = "";
    bool is_date_visible = false;
    int contract = 0;
public:
    Popup_Add_Student_To_Base(Graphical* graphical);
    bool show_frame();
    bool is_ok_possible() 
    {
        Student student;
        student.set_contract(contract);
        student.set_name(name);
        for (int i = 0; i < journal->student_count(); i++)
        {
            if (journal->student(i)->is_identical(student))
            {
                error("такой ученик уже существует");
                return false;
            }
        }
        return true;
    }
    void accept_changes();
};