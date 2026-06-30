#pragma once
#include "popup.h"

namespace View
{
    class Add_Student_To_Lesson : public Popup
    {
        Merged_Lesson_ID m_merged_lesson_id;
        std::vector<Position<Student>> m_merged_lesson_students;
        UI::Select_Students m_select_students;
    public:
        Add_Student_To_Lesson(const Shared& shared, IController& controller, Merged_Lesson_ID merged_lesson_id)
        : Popup(shared, "Добавление ученика в группу", controller),
          m_merged_lesson_id(merged_lesson_id),
          m_merged_lesson_students(model()->cref_merged_lesson(merged_lesson_id).get_student_positions()),
          m_select_students("##Выбрать_учеников", controller.model(), [&](Position<Student> student_pos) -> bool
        {
            return !std::any_of(m_merged_lesson_students.begin(), m_merged_lesson_students.end(), [student_pos](auto any_pos)
            {
                return student_pos == any_pos;
            });
        }, true)
        { }
        bool render_logic() override
        {
            m_select_students.render();
            return true;
        }
        std::vector<std::shared_ptr<ICommand>> get_actions() const override
        {
            std::vector<std::shared_ptr<ICommand>> output;
            for (auto student_pos : m_select_students.get_student_positions())
            {
                output.emplace_back(Ptr<::Add_Student_To_Lesson>::make(m_merged_lesson_id, student_pos));
            }
            return output;
        }
        std::optional<std::string> get_error() const
        {
            if (m_select_students.get_student_positions().size() == 0) return "Выберите ученика";
            return {};
        }
    };
}