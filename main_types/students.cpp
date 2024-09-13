#include "students.h"

void Students::update_contract_counter()
{
    _contract_counter.clear();
    for (const auto* student : _students)
    {
        if (student->is_removed()) continue;
        int contract = student->get_contract();
        _contract_counter[contract] = _contract_counter[contract] + 1;
    }
}

Students::Students(std::vector<Student *> students) : _students(students)
{
    update_contract_counter();
}

void Students::add_student(int contract, std::string name)
{
    _students.push_back(new Student(contract, name));
    update_contract_counter();
}

const Student* Students::student(int id) const
{
    return _students[id];
}

Students::StudentID Students::get_students_size() const
{
    return _students.size();
}

void Students::edit_student(StudentID id, Student::Contract contract, Student::Name name)
{
    auto& student = _students[id];
    bool should_update_students_counter = false;
    if (student->get_contract() != contract) should_update_students_counter = true;
    student->set_contract(contract);
    if (should_update_students_counter) update_contract_counter();
    student->set_name(name);
}

Students::Counter Students::get_student_counter(Student::Contract contract)
{
    return _contract_counter[contract];
}

std::vector<const Student*> Students::students() const
{
    std::vector<const Student*> output;
    for (int i = 0; i < _students.size(); i++)
    {
        output.push_back(_students[i]);
    }
    return output;
}