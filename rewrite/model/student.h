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
    Vector_Sortable<Contract>::Position m_contract_pos;
public:
    Student(std::string name, Vector_Sortable<Contract>::Position contract_pos)
    : m_name(name), m_contract_pos(contract_pos) { }
    Vector_Sortable<Contract>::Position get_contract_pos() const { return m_contract_pos; }
    void set_contract_pos(Vector_Sortable<Contract>::Position contract) { m_contract_pos = contract; }
    std::string get_name() const { return m_name; }
    void set_name(std::string name) { m_name = name; }
};