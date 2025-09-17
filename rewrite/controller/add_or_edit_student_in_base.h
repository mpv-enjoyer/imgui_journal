#pragma once
#include "icommand.h"

class Add_Or_Edit_Student_In_Base : public ICommand
{
    std::optional<Position<Student>> m_student_pos;
    std::string m_name;
    int m_contract;
public:
    Add_Or_Edit_Student_In_Base(std::string name, int contract)
    : m_name(name), m_contract(contract)
    { }
    Add_Or_Edit_Student_In_Base(Position<Student> student_pos, std::string name, int contract)
    : m_student_pos(student_pos), m_name(name), m_contract(contract)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        if (m_contract < 0) return "Неверный номер договора";
        if (m_name.size() == 0) return "Не задан ФИ";
        return {};
    }
    void call(IModel& model) override
    {
        if (m_student_pos)
        {
            
        }
        model->students()->add(m_name, m_contract);
    }
};