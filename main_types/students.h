#pragma once
#include "../main_types/main_types.h"
#include <map>

class Students
{
    std::map<int, int> _contract_counter;
    std::vector<Student*> _students;
    void update_contract_counter();
public:
    Students() { };
    Students(std::vector<Student*> students);
    void add_student(int contract, std::string name);
    std::vector<const Student*> students() const;
    const Student* student(int id) const;
    int get_students_size() const;
    void edit_student(int id, int contract, std::string name);
    int get_student_counter(int contract);
};