#pragma once

#include <string>
#include <vector>
#include "lib_time.h"
#include "lib_vector.h"
#include "contract.h"
#include "removal_info.h"
#include "automut.h"

class Student
{
    std::string m_name;
    Removal_Info m_removal_info;
    const Ptr<Contract>* m_contract;
public:
    Student(std::string name, const Ptr<Contract>& contract)
    : m_name(name), m_contract(&contract) { }
    const Removal_Info& cref_removal_info() const { return m_removal_info; }
    Removal_Info& ref_removal_info() { return m_removal_info; }
    const Ptr<Contract>& cref_contract() const { return *m_contract; }
    void set_contract(Ptr<Contract>& contract) { m_contract = &contract; }
    std::string get_name() const { return m_name; }
    void set_name(std::string name) { m_name = name; }
};