#pragma once

#include <string>
#include <vector>
#include "lib_time.h"
#include "lib_vector.h"
#include "contract.h"
#include "removal_info.h"
//using size_t = unsigned long long; //idk

class Student
{
    std::string m_name;
    Removal_Info m_removal_info;
    const std::unique_ptr<Contract>* m_contract;
public:
    Student(std::string name, const std::unique_ptr<Contract>& contract)
    : m_name(name), m_contract(&contract) { }
    Removal_Info& ref_removal_info() { return m_removal_info; }
    const std::unique_ptr<Contract>& cref_contract() { return *m_contract; }
    void set_contract(std::unique_ptr<Contract>& contract) { m_contract = &contract; }
    //struct LessonID
    //{
    //    Wday wday;
    //    std::size_t merged_lesson_info_id;
    //    std::size_t internal_lesson_info_id;
    //};
    //std::vector<LessonID> m_lessons;
};