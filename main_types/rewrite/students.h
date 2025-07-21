#pragma once

#include "student.h"
#include "contract.h"

class Students
{
    Vector<Contract> m_contracts;
    Vector<Student> m_students;
public:
    Students() { }
    void add(std::string name, int contract_id)
    {
        for (const auto& contract : m_contracts.data())
        {
            if (contract->get_id() == contract_id)
            {
                m_students.push_back(new Student(name, contract));
                return;
            }
        }
        auto& contract = m_contracts.push_back(new Contract(contract_id));
        m_students.push_back(new Student(name, contract));
    }
};