#pragma once
#include "subwindow.h"
#include "view/shared.h"
#include "controller/add_student_to_base.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        Shared& m_shared;
        UI::Button_Dangerous button = UI::Button_Dangerous("Add student", [&]()
        {
            m_controller.add(Ptr<Add_Student_To_Base>::make("Student Name", 42));
        });
        bool render_logic() override
        {
            ImGui::Text("%s", m_shared.edit_mode ? "edit mode" : "not edit mode");
            button.render();
            return !(ImGui::Button("Exit lol"));
        }
    public:
        Mainwindow(IController& controller, Shared& shared)
        : Subwindow(controller), m_shared(shared)
        { }
    };
}