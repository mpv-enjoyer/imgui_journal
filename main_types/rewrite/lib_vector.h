#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>
#include <numeric>
#include <functional>

template <typename T>
class Vector
{
public:
    class Position
    {
        std::size_t m_index;
    public:
        std::size_t get() const { return m_index; }
        explicit Position(std::size_t index) : m_index(index) { }
        Position(const Position& position) : m_index(position.m_index) { }
    };
private:
    using Tptr = std::unique_ptr<T>;
    using DataTypeBase = std::vector<Tptr>;
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
        Iterator(const Iterator<ValueType>& iterator)
        : m_data(iterator.data), m_max_index(iterator.data.size()), m_index(iterator.m_index)
        { }
        bool is_done() const
        {
            return m_index == m_max_index;
        }
        bool next()
        {
            assert(!is_done());
            ++m_index;
            return !is_done();
        }
        ValueType& get() const
        {
            assert(!is_done());
            return m_data.at(m_index);
        }
        Position get_position() const
        {
            assert(!is_done());
            return m_index;
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
            return indices;
        }
        std::size_t size() const
        {
            return m_indices.size();
        }
    public:
        IteratorSorted(DataTypeBase& data, std::function<bool(const T&, const T&)> compare)
        : m_data(data), m_compare(compare)
        {
            to_begin_update();
        }
        IteratorSorted(IteratorSorted<ValueType> it)
        : m_data(it.m_data), m_index(it.m_index), m_indices(it.m_indices), m_compare(it.m_compare)
        {
            to_begin_update();
        }
        bool is_done() const
        {
            return m_index == size();
        }
        bool next()
        {
            assert(!is_done());
            ++m_index;
            return !is_done();
        }
        ValueType& get() const
        {
            assert(!is_done());
            return m_data.at(m_indices[m_index]);
        }
        Position get_position() const
        {
            assert(!is_done());
            return m_indices[m_index];
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
    explicit Vector() = default;
    explicit Vector(std::size_t size)
    : m_data(size)
    { }
    ~Vector() = default;
    Vector(const Vector<T>&) = delete;
    Vector(Vector<T>&&) = delete;
    void operator=(Vector<T> const &t) = delete;
    Iterator<Tptr> begin()
    {
        return Iterator<Tptr>(m_data);
    }
    Iterator<const Tptr> cbegin() const
    {
        return Iterator<const Tptr>(m_data);
    }
    IteratorSorted<Tptr> sorted_begin(std::function<bool(const T&, const T&)> compare = [](const T& lhs, const T& rhs){ return lhs < rhs; })
    {
        return IteratorSorted<Tptr>(m_data, compare);
    }
    IteratorSorted<const Tptr> csorted_begin(std::function<bool(const T&, const T&)> compare = [](const T& lhs, const T& rhs){ return lhs < rhs; }) const
    {
        return IteratorSorted<const Tptr>(m_data, compare);
    }
    DataTypeBase& ref_data()
    {
        return m_data;
    }
    const DataTypeBase& cref_data() const
    {
        return m_data;
    }
    std::unique_ptr<T>& push_back(T* value)
    {
        m_data.push_back(std::unique_ptr<T>(value));
        return m_data.back();
    }
    Tptr& ref(const Position& position)
    {
        return m_data[position.get()];
    }
    const Tptr& cref(const Position& position) const
    {
        return m_data[position.get()];
    }
};