#include <vector>
#include <string>
#include <cassert>

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

using T = Student;
#include <list>
#include <array>
#include <cassert>
#include <functional>
// Not robust aggregate: iterators get messed up after removal or insert
// DO NOT USE FOR POINTERS AND REFERENCES
//template <typename T = int>
class Aggregate
{
private:
    static_assert(!std::is_pointer<T>());

    std::vector<T> m_data;
    std::list<std::size_t> m_sorted;
    std::size_t m_version = 0;

    struct IteratorSortedData
    {
        std::size_t version;
        std::list<std::size_t>::const_iterator pos;
    };
    void check(IteratorSortedData data) const
    {
        assert(data.version == m_version);
    };
    IteratorSortedData reset() const;
    IteratorSortedData next(IteratorSortedData data) const;
    const T& get(IteratorSortedData data) const;
    std::vector<T>::const_iterator get_iterator(IteratorSortedData data);
    bool is_done(IteratorSortedData data) const;

    class IteratorSorted
    {
        IteratorSortedData m_pos;
        Aggregate* m_iterable;
    public:
        IteratorSorted(Aggregate* iterable) : m_iterable(iterable)
        {
            reset();
        }
        void reset()
        {
            m_pos = m_iterable->reset();
        }
        bool is_done()
        {
            return m_iterable->is_done(m_pos);
        }
        void next()
        {
            assert(!is_done());
            m_pos = m_iterable->next(m_pos);
        }
        const T& get()
        {
            assert(!is_done());
            return m_iterable->get(m_pos);
        }
        const std::vector<T>::const_iterator& common_iterator()
        {
            assert(!is_done());
            return m_iterable->get_iterator(m_pos);
        }
    };
public:
    Aggregate() { };
    Aggregate(Aggregate& aggregate) = delete;
    std::vector<T>::const_iterator begin() const
    {
        return m_data.cbegin();
    }
    IteratorSorted begin_sorted()
    {
        return IteratorSorted(this);
    }
    void push_back(T&& value)
    {
        m_version++;
        std::size_t id_new = m_data.size();
        m_data.push_back(value);
        for (auto it = m_sorted.begin(); it != m_sorted.end(); ++it)
        {
            std::size_t data_id = *it;
            if (m_data[data_id] > value)
            {
                m_sorted.insert(it, id_new);
                return;
            }
        }
        m_sorted.insert(m_sorted.end(), id_new);
    }
    void modify(const std::vector<T>::const_iterator& iter, std::function<void(T&)> modify_func)
    {
        assert(m_data.cend() != iter);
        std::size_t index = std::distance(m_data.cbegin(), iter);
        auto& to_modify = *(m_data.begin() + index);
        modify_func(to_modify);
    }
    std::size_t size()
    {
        return m_data.size();
    }
};

inline auto Aggregate::reset() const -> IteratorSortedData
{
    return { m_version, m_sorted.cbegin() };
}
inline auto Aggregate::next(IteratorSortedData data) const -> IteratorSortedData
{
    check(data);
    data.pos++;
    return data;
}
inline const T& Aggregate::get(IteratorSortedData data) const
{
    check(data);
    auto id = *(data.pos);
    return m_data[id];
}
inline std::vector<T>::const_iterator Aggregate::get_iterator(IteratorSortedData data)
{
    check(data);
    auto id = *(data.pos);
    return m_data.cbegin() + id;
}
inline bool Aggregate::is_done(IteratorSortedData data) const
{
    check(data);
    return data.pos == m_sorted.end();
}

#include <memory>

/* REMAKE Aggregate without version and in-place sorting */
class Vector
{
    using T = Student;
    using DataTypeBase = std::vector<std::unique_ptr<T>>;
    DataTypeBase m_data;
    template <typename DataType>
    class Iterator
    {
        DataType& m_data;
        std::size_t m_max_index;
        std::size_t m_index = 0;
    public:
        Iterator(DataType& data)
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
        typename DataType::value_type& get()
        {
            assert(!is_done());
            return m_data.at(m_index);
        }
        void update()
        {
            m_max_index = m_data.size();
        }
    };
public:
    Iterator<DataTypeBase> begin()
    {
        return Iterator<DataTypeBase>(m_data);
    }
    Iterator<const DataTypeBase> begin_const()
    {
        return Iterator<const DataTypeBase>(m_data);
    }
    void push_back(T* value)
    {
        m_data.push_back(std::unique_ptr<T>(value));
    }
};

#include <iostream>

int main()
{
    Vector values;
    values.push_back(new Student(4));
    values.push_back(new Student(2));
    auto iter1 = values.begin();
    do
    {
        iter1.get()->set_name("new name");
        std::cout << "iter1 iterated\n";
    }
    while (iter1.next());
    Student* g = new Student(1);
    g->set_name("something_new");
    values.push_back(g);
    for (auto iter2 = values.begin_const(); !iter2.is_done(); iter2.next())
    {
        std::cout << iter2.get()->get_name() << "\n";
    }

    //std::vector<int> values;
}