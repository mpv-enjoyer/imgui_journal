#pragma once
#include "view/ui/ui.h"
#include "model/lesson_infos.h"

namespace UI
{
    class Select_Lesson_Type : public AUnit
    {
    private:
        const Combobox_Choices<std::vector<Lesson_Type>> M_CHOICES =
        {
            {"ИЗО", { Lesson_Type::DRAWING }},
            {"Лепка", { Lesson_Type::SCULPTING }},
            {"ИЗО+Лепка", { Lesson_Type::DRAWING, Lesson_Type::SCULPTING }},
            {"Лепка+ИЗО", { Lesson_Type::SCULPTING, Lesson_Type::DRAWING }},
            {"Дизайн", { Lesson_Type::DESIGN }},
            {"Черчение", { Lesson_Type::TECHDRAWING }},
            {"Спецкурс", { Lesson_Type::SPECIALCOURSE }}
        };
        int get_choice_from_provided(const Attendance_Merged_Lesson* merged_lesson) const
        {
            if (!merged_lesson) return 0;
            std::vector<Lesson_Type> lesson_types;
            for (const auto& internal_lesson : merged_lesson->cref_internal_lessons())
            {
                lesson_types.push_back(internal_lesson.get_lesson_type());
            }
            for (int i = 0; i < M_CHOICES.size(); i++)
            {
                if (lesson_types == M_CHOICES[i].value) return i;
            }
            return 0;
        }

        Combobox<std::vector<Lesson_Type>> m_combo;
        std::vector<Lesson_Type> m_values;
        std::function<void (std::vector<Lesson_Type>)> m_callback;
        void render_logic() override
        {
            m_combo.render();
        }
    public:
        Select_Lesson_Type(std::string id, std::function<bool (std::vector<Lesson_Type>)> callback, const Attendance_Merged_Lesson* merged_lesson = nullptr)
        : AUnit(id), m_combo(id, M_CHOICES, get_choice_from_provided(merged_lesson), callback) { }
        std::vector<Lesson_Type> get_lesson_types() const
        {
            return m_combo.get_choice();
        }
        void trigger_callback()
        {
            m_combo.trigger_callback();
        }
    };
}