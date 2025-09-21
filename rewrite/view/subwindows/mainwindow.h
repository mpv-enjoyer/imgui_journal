#pragma once
#include "subwindow.h"
#include "view/shared.h"
#include "controller/commands.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        UI::Input_Int input_int = UI::Input_Int("Student contract here", 0);
        Shared& m_shared;
        UI::Button_Dangerous button = UI::Button_Dangerous("Add student", [&]()
        {
            m_controller.add(Ptr<Add_Or_Edit_Student_In_Base>::make("Student Name", input_int.get_value()));
        });
        UI::Button_Dangerous button2 = UI::Button_Dangerous("Add Group", [&]()
        {
            std::vector<Add_Or_Edit_Merged_Lesson::Request> requests = {
                {
                    .type = Attendance_Internal_Lesson::Type::DESIGN,
                    .begin = JTime(10, 40),
                    .end = JTime(11, 50)
                }
            };
            m_controller.add(Ptr<Add_Or_Edit_Merged_Lesson>::make(Wday::make_current(), 0, "comment", 4, requests));
        });
        bool render_logic() override
        {
            input_int.render();
            ImGui::Text("%s", m_shared.edit_mode ? "edit mode" : "not edit mode");
            button.render();
            for (auto it = m_model->students()->cref_students().cbegin(); it; it.next())
            {
                auto contract = m_model->students()->cref_contracts().cref(it->get_contract_pos());
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