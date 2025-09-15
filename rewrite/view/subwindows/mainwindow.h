#pragma once
#include "subwindow.h"
#include "view/shared.h"
#include "controller/commands.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        Shared& m_shared;
        UI::Button_Dangerous button = UI::Button_Dangerous("Add student", [&]()
        {
            m_controller.add(Ptr<Add_Student_To_Base>::make("Student Name", 42));
        });
        UI::Button_Dangerous button2 = UI::Button_Dangerous("Add Group", [&]()
        {
            std::vector<Add_Merged_Lesson::Request> requests = {
                {
                    .type = Attendance_Internal_Lesson::Type::DESIGN,
                    .begin = JTime(10, 40),
                    .end = JTime(11, 50)
                }
            };
            m_controller.add(Ptr<Add_Merged_Lesson>::make(Wday::make_current(), 0, "comment", 4, requests));
        });
        bool render_logic() override
        {
            ImGui::Text("%s", m_shared.edit_mode ? "edit mode" : "not edit mode");
            button.render();
            for (auto it = m_model->students()->cref_students().cbegin(); it; it.next())
            {
                auto contract = m_model->students()->cref_contracts().cref(it->contract_pos());
                ImGui::Text("student %s contract %i", it->get_name().c_str(), contract.get_id());
            }
            button2.render();
            return ImGui::Button("Exit lol");
        }
    public:
        Mainwindow(IController& controller, Shared& shared)
        : Subwindow("mainwindow", controller), m_shared(shared)
        { }
    };
}