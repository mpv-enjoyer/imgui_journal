#include <vector>
#include <string>
#include <cassert>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>

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

/* ---------------------- */
/* END OF STUDENT DEFINES */
/* ---------------------- */

/* REMAKE Aggregate without version and in-place sorting */
template <typename T>
class Vector
{
    using Tptr = std::unique_ptr<T>;
    using DataTypeBase = std::vector<std::unique_ptr<T>>;
    DataTypeBase m_data;
    template <typename ValueType>
    class Iterator
    {
        DataTypeBase& m_data;
        std::size_t m_max_index;
        std::size_t m_index = 0;
    public:
        Iterator(DataTypeBase& data)
        : m_data(data), m_max_index(data.size())
        { }
        bool is_done()
        {
            return m_index == m_max_index;
        }
        bool next()
        {
            assert(!is_done());
            ++m_index;
            return !is_done();
        }
        ValueType& get()
        {
            assert(!is_done());
            return m_data.at(m_index);
        }
        void update()
        {
            m_max_index = m_data.size();
        }
    };
    template <typename ValueType>
    class IteratorSorted
    {
        DataTypeBase& m_data;
        std::size_t m_index = 0;
        std::vector<std::size_t> m_indices;
        std::function<bool(const T&, const T&)> m_compare = [](const T& lhs, const T& rhs){ return lhs < rhs; };
        // https://stackoverflow.com/a/10581051
        std::vector<std::size_t> get_ordered(std::vector<std::unique_ptr<T>> const& values) {
            std::vector<std::size_t> indices(values.size());
            std::iota(std::begin(indices), std::end(indices), static_cast<std::size_t>(0));

            std::sort(
                std::begin(indices), std::end(indices),
                [&](std::size_t l, std::size_t r)
                {
                    // TT is a unique_ptr. We want to compare values that it holds.
                    const auto& lhs = *(values[l]);
                    const auto& rhs = *(values[r]);
                    return m_compare(lhs, rhs);
                }
            );
            //std::cout << "[DEBUG] indices: ";
            //for (std::size_t i = 0; i < indices.size(); i++)
            //{
            //    std::cout << " " << indices[i];
            //}
            //std::cout << "\n";
            return indices;
        }
        std::size_t size()
        {
            return m_indices.size();
        }
    public:
        IteratorSorted(DataTypeBase& data, std::function<bool(const T&, const T&)> compare)
        : m_data(data), m_compare(compare)
        {
            to_begin_update();
        }
        bool is_done()
        {
            return m_index == size();
        }
        bool next()
        {
            assert(!is_done());
            ++m_index;
            return !is_done();
        }
        ValueType& get()
        {
            assert(!is_done());
            return m_data.at(m_indices[m_index]);
        }
        void to_begin()
        {
            m_index = 0;
        }
        void to_begin_update()
        {
            to_begin();
            m_indices = get_ordered(m_data);
        }
    };
public:
    Iterator<Tptr> begin()
    {
        return Iterator<Tptr>(m_data);
    }
    Iterator<const Tptr> begin_const()
    {
        return Iterator<const Tptr>(m_data);
    }
    IteratorSorted<Tptr> sorted_begin(std::function<bool(const T&, const T&)> compare = [](const T& lhs, const T& rhs){ return lhs < rhs; })
    {
        return IteratorSorted<Tptr>(m_data, compare);
    }
    IteratorSorted<const Tptr> sorted_begin_const(std::function<bool(const T&, const T&)> compare = [](const T& lhs, const T& rhs){ return lhs < rhs; })
    {
        return IteratorSorted<const Tptr>(m_data, compare);
    }
    void push_back(T* value)
    {
        m_data.push_back(std::unique_ptr<T>(value));
    }
};


int main()
{
    Vector<Student> values;
    values.push_back(new Student(4));
    values.push_back(new Student(2));
    auto iter1 = values.begin();
    do
    {
        iter1.get()->set_name("name2");
        std::cout << "iter1 iterated\n";
    }
    while (iter1.next());
    Student* g = new Student(1);
    g->set_name("name1");
    values.push_back(g);
    for (auto iter2 = values.begin_const(); !iter2.is_done(); iter2.next())
    {
        std::cout << iter2.get()->get_name() << "\n";
    }

    std::cout << iter1.is_done() << "\n";
    iter1.update();
    std::cout << iter1.is_done() << "\n";
    std::cout << iter1.next() << "\n";
    //std::vector<int> values;

    auto iter_sorted = values.sorted_begin_const();
    do
    {
        auto& val = iter_sorted.get();
        std::cout << val->get_name() << "\n";
    } while (iter_sorted.next());

    iter_sorted.to_begin_update();

    values.begin().get()->set_name("xxxx");
    g = new Student(1);
    g->set_name("zzzz");
    values.push_back(g);
    do
    {
        auto& val = iter_sorted.get();
        std::cout << val->get_name() << "\n";
    } while (iter_sorted.next());

    iter_sorted.to_begin_update();
    do
    {
        auto& val = iter_sorted.get();
        std::cout << val->get_name() << "\n";
    } while (iter_sorted.next());

    std::cout << "\n\nBackwards iteration:\n";
    auto iter = values.sorted_begin_const([](const auto& l, const auto& r){ return l > r; });
    do
    {
        auto& val = iter.get();
        std::cout << val->get_name() << "\n";
    } while (iter.next());

    for (auto it : values)
    {
        
    }

}