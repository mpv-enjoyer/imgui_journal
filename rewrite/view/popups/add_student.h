#pragma once
#include "popup.h"

namespace View
{
    class Popup_Add_Student_To_Base : public Popup
    {
        UI::Input_Text m_name = UI::Input_Text("ФИ ученика", "");
        UI::Input_Int m_contract = UI::Input_Int("Номер договора");
    public:
        Popup_Add_Student_To_Base(const Shared& shared, IController& controller)
        : Popup(shared, "Добавить ученика в базу", controller)
        { }

        bool render_logic() override
        {
            m_name.render();
            m_contract.render();
            return true;
        }
        std::vector<std::shared_ptr<ICommand>> get_actions() const override
        {
            std::vector<std::shared_ptr<ICommand>> output = { std::make_shared<Add_Student_To_Base>(m_name.get_value(), m_contract.get_value()) };
            return output;
        }
        std::optional<std::string> get_error() const override
        {
            if (m_name.get_value().size() == 0) return "введите имя ученика";
            for (const auto& student : model()->students()->cref_students())
            {
                if (student.get_name() == m_name.get_value() &&
                    model()->students()->cref_contracts()[student.get_contract_pos()].get_number() == m_contract.get_value())
                {
                    return "такой ученик уже существует";
                }
            }
            return {};
        }
    };
} // namespace View

