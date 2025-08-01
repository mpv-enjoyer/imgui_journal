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
        for (auto& contract : m_contracts.cref_data())
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
    const Vector<Contract>& cref_contracts() const
    {
        return m_contracts;
    }
    const Vector<Student>& cref_students() const
    {
        return m_students;
    }
};