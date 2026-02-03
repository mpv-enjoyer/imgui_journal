#pragma once
#include <string>
#include <vector>
#include "jtime.h"
#include "common/vector_sortable.h"
#include "common/modifiers.h"
#include "contract.h"
#include "common/removal_info.h"

class Student : public Removal_Info
{
    std::string m_name;
    Pos<Contract> m_contract_pos;
public:
    Student(std::string name, Pos<Contract> contract_pos)
    : m_name(name), m_contract_pos(contract_pos) { }
    Pos<Contract> get_contract_pos() const { return m_contract_pos; }
    void set_contract_pos(Pos<Contract> contract) { m_contract_pos = contract; }
    std::string get_name() const { return m_name; }
    void set_name(std::string name) { m_name = name; }
};