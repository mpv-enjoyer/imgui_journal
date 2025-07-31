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
    const Vector<Contract>& contracts() const
    {
        return m_contracts;
    }
    const Vector<Student>& students() const
    {
        return m_students;
    }
    const std::unique_ptr<Contract>& contract(Vector<Contract>::Position position) const
    {
        return m_contracts.cref(position);
    }
    std::unique_ptr<Contract>& contract_mut(Vector<Contract>::Position position)
    {
        return m_contracts.ref(position);
    }
    const std::unique_ptr<Student>& student(Vector<Student>::Position position) const
    {
        return m_students.cref(position);
    }
    std::unique_ptr<Student>& student_mut(Vector<Student>::Position position)
    {
        return m_students.ref(position);
    }
};