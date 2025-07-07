#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

class Student
{
private:
    const std::size_t ID;
    bool removed = 0;
    int contract;
    std::string name;
public:
    Student(std::size_t id);
    int get_contract() const; bool set_contract(int new_contract);
    std::string get_name() const; bool set_name(std::string new_name);
    bool is_removed() const; bool remove(); bool restore();
    std::size_t get_id() const;
    bool is_identical(const Student& rhs) const; //this is an entry check. Once there is an identical student for some reason, do nothing about it.
    bool operator==(const Student& rhs) const;
    bool operator!=(const Student& rhs) const;
    bool operator< (const Student& rhs) const;
    bool operator> (const Student& rhs) const;
};

Student::Student(std::size_t id)
: ID(id)
{
    contract = -1;
}

int Student::get_contract() const
{
    return contract;
}

std::string Student::get_name() const
{
    return name;
}

bool Student::set_contract(int new_contract)
{
    contract = new_contract;
    return true;
}

bool Student::set_name(std::string new_name)
{
    if (new_name.length()==0) return false;
    name = new_name;
    return true;
}

bool Student::is_removed() const
{
    return removed;
}

std::size_t Student::get_id() const
{
    return ID;
}

bool Student::remove()
{
    removed = true;
    return true;
}

bool Student::restore()
{
    removed = false;
    return true;
}

bool Student::is_identical(const Student& rhs) const
{
    if (removed != rhs.removed) return false;
    if (name == rhs.name && contract == rhs.contract) return true;
    return false;
}

bool Student::operator==(const Student& rhs) const { return this == &rhs; }
bool Student::operator!=(const Student& rhs) const { return !(this == &rhs); }
bool Student::operator< (const Student& rhs) const { return std::tie(contract, name) < std::tie(rhs.contract, rhs.name); }
bool Student::operator> (const Student& rhs) const { return rhs < *this; }

class AllStudents
{
    struct name {};
    using Dataset = boost::multi_index_container<
        Student,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
                boost::multi_index::const_mem_fun<Student, std::size_t, &Student::get_id>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<name>,
                boost::multi_index::const_mem_fun<Student, std::string, &Student::get_name>
            >
        >
    >;
    Dataset data;
public:
    using IteratorID = decltype(data.get<0>().begin());
    IteratorID get_id_iterator()
    {
        struct Functor
        {
            std::string name_; 
            Functor(const std::string& name) : name_(name) { };
            void operator()(Student& s)
            {
                s.set_name(name_);
            }
        };
        Dataset::nth_index<1>::type& by_id = data.get<1>();
        by_id.modify(by_id.begin(), Functor("Changed name"));
        //data.modify(data.get<0>().begin(), Functor("name"));
        return data.get<0>().begin();
    }
    AllStudents()
    {
        data.insert(Student(0));
    }
};

#include <iostream>

int main()
{
    AllStudents as;
    auto iter = as.get_id_iterator();
    std::cout << iter->get_name() << "\n";
}