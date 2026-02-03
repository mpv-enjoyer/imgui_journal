#pragma once
#include "icommand.h"

class Add_Or_Edit_Student_In_Base : public ICommand
{
    std::optional<Pos<Student>> m_student_pos;
    std::string m_name;
    int m_contract;
    std::string name_with_postfix_if_needed(const IModel& model) const
    {
        std::string name_to_set = m_name;
        int postfix_iter = 0;
        bool student_found = false;
        do
        {
            student_found = false;
            name_to_set = m_name;
            if (postfix_iter != 0) name_to_set += " (" + std::to_string(postfix_iter) + ")";
            for (auto it = model->students()->cref_students().begin(); it; ++it)
            {
                if (m_student_pos && *m_student_pos == it.get_position()) continue;
                if (it->is_removed()) continue;
                if (model->students()->cref_contracts()[it->get_contract_pos()].get_number() != m_contract) continue;
                if (it->get_name() != name_to_set) continue;
                student_found = true;
                break;
            }
            postfix_iter++;
        } while (student_found);
        return name_to_set;
    }
public:
    Add_Or_Edit_Student_In_Base(std::string name, int contract)
    : m_name(name), m_contract(contract)
    { }
    Add_Or_Edit_Student_In_Base(Pos<Student> student_pos, std::string name, int contract)
    : m_student_pos(student_pos), m_name(name), m_contract(contract)
    { }
    Error get_error(const IModel& model) const override
    {
        if (m_contract < 0) return "Неверный номер договора";
        if (m_name.size() == 0) return "Не задан ФИ";
        return {};
    }
    void call(IModel& model) override
    {
        std::string name_to_set = name_with_postfix_if_needed(model);
        if (m_student_pos)
        {
            model->students()->edit(*m_student_pos, name_to_set, m_contract);
        }
        else
        {
            model->students()->add(name_to_set, m_contract);
        }
    }
};