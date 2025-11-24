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
        UI::Button button = UI::Button("Add student", [&]()
        {
            m_controller.add(Ptr<Add_Or_Edit_Student_In_Base>::make("Student Name", input_int.get_value()));
        }, UI::Button::Colors::Dangerous);
        UI::Select_Lesson_Type select_lesson_type = UI::Select_Lesson_Type("Select lesson type", nullptr);
        UI::Button button2 = UI::Button("Add Group", [&]()
        {
            std::vector<Add_Or_Edit_Merged_Lesson::Request> requests = {
                {
                    .type = select_lesson_type.get_lesson_types().front(),
                    .begin = begin_jtime.get_value(),
                    .end = end_jtime.get_value()
                }
            };
            m_controller.add(Ptr<Add_Or_Edit_Merged_Lesson>::make(Mday::make_current(), 0, "comment", 4, requests));
        }, UI::Button::Colors::Dangerous);
        UI::Input_JTime begin_jtime = UI::Input_JTime("##begin");
        UI::Input_JTime end_jtime = UI::Input_JTime("##end");
        std::unique_ptr<UI::Select_Students> select_students;
        UI::Checkbox checkbox = UI::Checkbox("Select multiple?");
        bool render_logic() override
        {
            input_int.render();
            UI::label(m_shared.edit_mode ? "edit mode" : "not edit mode");
            button.render();
            for (auto it = m_model->students()->cref_students().cbegin(); it; it.next())
            {
                int contract_number = m_model->students()->get_contract_number(it.get_position());
                UI::label("student " + it->get_name() + " contract " + std::to_string(contract_number));
            }
            button2.render();
            begin_jtime.render();
            ImGui::SameLine();
            UI::label(" _ ");
            ImGui::SameLine();
            end_jtime.render();
            checkbox.render();
            if (ImGui::Button("Select Students"))
            {
                select_students.reset(new UI::Select_Students("Select Students", m_controller.model(), [&](Position<Student> pos)
                {
                    return pos.get() % 2 == 1;
                }, checkbox.get_value()));
            }
            if (select_students) select_students->render();
            else select_lesson_type.render();
            return ImGui::Button("Exit lol");
        }
    public:
        Mainwindow(IController& controller, Shared& shared)
        : Subwindow("mainwindow", controller), m_shared(shared)
        { }
    };
}