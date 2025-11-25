#pragma once
#include "subwindow_handler.h"
#include "view/shared.h"
#include "controller/commands/commands.h"

namespace View
{
    class Testwindow : public Subwindow
    {
        UI::Input_Int input_int = UI::Input_Int("Student contract here", 0);
        UI::Button button = UI::Button("Add student", [&]()
        {
            controller().add(Ptr<Add_Or_Edit_Student_In_Base>::make("Student Name", input_int.get_value()));
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
            controller().add(Ptr<Add_Or_Edit_Merged_Lesson>::make(Mday::make_current(), 0, "comment", 4, requests));
        }, UI::Button::Colors::Dangerous);
        UI::Input_JTime begin_jtime = UI::Input_JTime("##begin");
        UI::Input_JTime end_jtime = UI::Input_JTime("##end");
        std::unique_ptr<UI::Select_Students> select_students;
        UI::Checkbox checkbox = UI::Checkbox("Select multiple?");
        UI::Calendar calendar = UI::Calendar("I am a calendar", Month::make_begin_study_year(Month::make_current()), [](Mday mday)
        {
            return mday.get_from_0() % 3 == 1;
        });
        bool render_logic() override
        {
            if (ImGui::Button("Добавить учеников в первый урок"))
            {
                Merged_Lesson_ID merged_lesson_id(Wday::make_current(), Position<Attendance_Merged_Lesson>(0));
                popup_handler().open_popup(std::make_unique<Add_Student_To_Lesson>(controller(), merged_lesson_id));
            }
            input_int.render();
            UI::label(shared().edit_mode ? "edit mode" : "not edit mode");
            button.render();
            for (auto it = model()->students()->cref_students().cbegin(); it; it.next())
            {
                int contract_number = model()->students()->get_contract_number(it.get_position());
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
                select_students.reset(new UI::Select_Students("Select Students", controller().model(), [&](Position<Student> pos)
                {
                    return pos.get() % 2 == 1;
                }, checkbox.get_value()));
            }
            if (select_students) select_students->render();
            else select_lesson_type.render();
            calendar.render();
            return ImGui::Button("Exit lol");
        }
    public:
        Testwindow(IController& controller, Shared& shared)
        : Subwindow("mainwindow", controller, shared)
        { }
    };
}