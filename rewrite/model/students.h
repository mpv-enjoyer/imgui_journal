#pragma once

#include "student.h"
#include "contract.h"

class Students
{
    Vector_Sortable<Contract> m_contracts;
    Vector_Sortable<Student> m_students;
public:
    Students() { }
    void add(std::string name, int contract_id)
    {
        for (auto it = m_contracts.cbegin(); it; it.next())
        {
            auto& contract = it.get();
            if (contract->get_id() == contract_id)
            {
                m_students.push_back(new Student(name, it.get_position()));
                return;
            }
        }
        auto position = m_contracts.push_back(new Contract(contract_id));
        m_students.push_back(new Student(name, position));
    }
    const Vector_Sortable<Contract>& cref_contracts() const
    {
        return m_contracts;
    }
    const Vector_Sortable<Student>& cref_students() const
    {
        return m_students;
    }
};