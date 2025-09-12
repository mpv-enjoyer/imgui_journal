#pragma once
#include "icommand.h"

class Add_Student_To_Base : public ICommand
{
    std::string m_name;
    int m_contract;
public:
    Add_Student_To_Base(std::string name, int contract)
    : m_name(name), m_contract(contract)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        if (m_contract < 0) return "Неверный номер договора";
        if (m_name.size() == 0) return "Не задан ФИ";
        return {};
    }
    void call(IModel& model)
    {
        model->students()->add(m_name, m_contract);
    }
};