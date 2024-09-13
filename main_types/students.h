#pragma once
#include "../main_types/main_types.h"
#include <map>

class Students
{
public:
    NO_IMPLICIT_CONVERSION_T(int, Counter);
    NO_IMPLICIT_CONVERSION_T(std::size_t, StudentID);
private:
    std::map<Student::Contract, Counter> _contract_counter;
    std::vector<Student*> _students;
    void update_contract_counter();
public:
    Students() { };
    Students(std::vector<Student*> students);
    void add_student(int contract, std::string name);
    std::vector<const Student*> students() const;
    const Student* student(int id) const;
    StudentID get_students_size() const;
    void edit_student(StudentID id, Student::Contract contract, Student::Name name);
    Counter get_student_counter(Student::Contract contract);
};