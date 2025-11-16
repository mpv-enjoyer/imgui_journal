#pragma once
#include "subwindow.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Mainwindow : public Subwindow
    {
        UI::Input_Int input_int = UI::Input_Int("Student contract here", 0);
        Shared& m_shared;
        UI::Button button = UI::Button("Add student", nullptr, [&]()
        {
            m_controller.add(Ptr<Add_Or_Edit_Student_In_Base>::make("Student Name", input_int.get_value()));
        }, UI::Button::Colors::Dangerous);
        UI::Button button2 = UI::Button("Add Group", nullptr, [&]()
        {
            std::vector<Add_Or_Edit_Merged_Lesson::Request> requests = {
                {
                    .type = Attendance_Internal_Lesson::Type::DESIGN,
                    .begin = JTime(10, 40),
                    .end = JTime(11, 50)
                }
            };
            m_controller.add(Ptr<Add_Or_Edit_Merged_Lesson>::make(Wday::make_current(), 0, "comment", 4, requests));
        }, UI::Button::Colors::Dangerous);
        bool render_logic() override
        {
            input_int.render();
            ImGui::Text("%s", m_shared.edit_mode ? "edit mode" : "not edit mode");
            button.render();
            for (auto it = m_model->students()->cref_students().cbegin(); it; it.next())
            {
                int contract_number = m_model->students()->get_contract_number(it.get_position());
                ImGui::Text("student %s contract %i", it->get_name().c_str(), contract_number);
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