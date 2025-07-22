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
        for (auto& contract : m_contracts.data())
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
        return m_contracts.data().at(position.get());
    }
    std::unique_ptr<Contract>& contract_mut(Vector<Contract>::Position position)
    {
        return m_contracts.data_mut().at(position.get());
    }
    const std::unique_ptr<Student>& student(Vector<Student>::Position position) const
    {
        return m_students.data().at(position.get());
    }
    std::unique_ptr<Student>& student_mut(Vector<Student>::Position position)
    {
        return m_students.data_mut().at(position.get());
    }
};