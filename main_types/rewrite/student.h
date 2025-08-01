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
    const std::unique_ptr<Contract>* m_contract;
public:
    Student(std::string name, const std::unique_ptr<Contract>& contract)
    : m_name(name), m_contract(&contract) { }
    const Removal_Info& cref_removal_info() const { return m_removal_info; }
    AUTOMUT0(ref_removal_info)
    const std::unique_ptr<Contract>& cref_contract() const { return *m_contract; }
    void set_contract(std::unique_ptr<Contract>& contract) { m_contract = &contract; }
};