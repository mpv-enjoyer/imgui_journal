#include "confirm_delete_student.h"

Popup_Confirm_Delete_Student::Popup_Confirm_Delete_Student(Graphical *_graphical, int student_id)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    _student_id = student_id;
}

bool Popup_Confirm_Delete_Student::show_frame()
{
    const Student* student = journal->student(_student_id);
    IM_ASSERT(!student->is_removed());
    if (begin_frame("Удалить ученика?"))
    {
        ImGui::Text(("Номер договора: " + std::to_string(student->get_contract())).c_str());
        ImGui::Text(("ФИ: " + student->get_name()).c_str());
        if (ImGui::Button("OK") && is_ok_possible()) POPUP_OK;
        ImGui::SameLine();
        if (ImGui::Button("Отмена") || should_exit()) POPUP_CANCEL;
        ImGui::SameLine(); print_error();
        ImGui::EndPopup();
    }
    return false;
}

void Popup_Confirm_Delete_Student::accept_changes()
{
    IM_ASSERT(check_ok());
    journal->remove_student(_student_id);
}
