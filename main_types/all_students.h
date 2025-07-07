#pragma once
#include <student.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

class AllStudents
{
    using Dataset = boost::multi_index_container<
        Student,
        boost::multi_index::indexed_by<
            boost::multi_index::const_mem_fun<Student, std::size_t, &Student::get_id>,
            boost::multi_index::const_mem_fun<Student, std::string, &Student::get_name>
        >
    >;
    Dataset data;
public:
    Dataset::nth_index_const_iterator<0> get_id_iterator() const;
    void change_name();
};