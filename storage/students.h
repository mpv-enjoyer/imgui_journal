#pragma once
#include "../main_types/main_types.h"
#include <map>

class Students
{
    std::map<int, int> contract_counter;
    std::vector<Student*> students;
    void update_contract_counter();
public:
    Students() { };
    void add_student(int contract, std::string name);
    std::vector<const Student*> get_students() const;
    const Student* get_student(int id) const;
    void edit_student(int id, int contract, std::string name);
};

