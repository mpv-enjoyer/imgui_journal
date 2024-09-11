#include "students.h"

void Students::update_contract_counter()
{
    contract_counter.clear();
    for (const auto* student : students)
    {
        int contract = student->get_contract();
        contract_counter[contract]++;
    }
}

void Students::add_student(int contract, std::string name)
{
    students.push_back(new Student(contract, name));
    update_contract_counter();
}

const Student* Students::get_student(int id) const
{
    return students[id];
}

std::vector<const Student*> Students::get_students() const
{
    std::vector<const Student*> output;
    for (int i = 0; i < students.size(); i++)
    {
        output.push_back(students[i]);
    }
    return output;
}

void Students::edit_student(int id, int contract, std::string name)
{
    auto& student = students[id];
    bool should_update_students_counter = false;
    if (student->get_contract() != contract) should_update_students_counter = true;
    student->set_contract(contract);
    if (should_update_students_counter) update_contract_counter();
    student->set_name(name); 
}