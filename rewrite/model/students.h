#pragma once

#include "student.h"
#include "contract.h"

class Students
{
    Vector_Sortable<Contract> m_contracts;
    Vector_Sortable<Student> m_students;
    Position<Contract> insert_contract_if_not_exists(int contract_id)
    {
        for (auto it = m_contracts.begin(); it; ++it)
        {
            if (it->get_id() == contract_id) return it.get_position();
        }
        return m_contracts.push_back(Ptr<Contract>::make(contract_id));
    }
public:
    Students() { }
    void add(std::string name, int contract_id)
    {
        auto position = insert_contract_if_not_exists(contract_id);
        m_students.push_back(Ptr<Student>::make(name, position));
    }
    void edit(Position<Student> student_pos, std::string name, int contract_id)
    {
        auto& student = m_students.ref(student_pos);
        student.set_contract_pos(insert_contract_if_not_exists(contract_id));
        student.set_name(name);
    }
    Removal_Info& ref_removal_info(Position<Student> student_pos)
    {
        return m_students.ref(student_pos).ref_removal_info();
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