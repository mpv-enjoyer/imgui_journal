#pragma once
#include <vector>
#include <cassert>
#include "ptr.h"
#include <algorithm>
#include <numeric>
#include <functional>
#include "modifiers.h"

template <typename T>
class Vector_Sortable
{
    NON_COPYABLE_NOR_MOVABLE(Vector_Sortable);
public:
    class Position
    {
        std::size_t m_index;
    public:
        std::size_t get() const { return m_index; }
        explicit Position(std::size_t index) : m_index(index) { }
        Position(const Position& position) : m_index(position.m_index) { }
        bool operator==(const Position& other) const { return m_index == other.m_index; }
        bool operator!=(const Position& other) const { return !(*this == other); }
    };
private:
    using Tptr = Ptr<T>;
    using DataTypeBase = std::vector<Tptr>;
    DataTypeBase m_data;
    template <typename ValueType, typename DataTypeBase>
    class Iterator
    {
        DataTypeBase& m_data;
        std::size_t m_max_index;
        std::size_t m_index = 0;
    public:
        Iterator(DataTypeBase& data)
        : m_data(data), m_max_index(data.size())
        { }
        Iterator(const Iterator<ValueType, DataTypeBase>& iterator)
        : m_data(iterator.m_data), m_max_index(iterator.m_max_index), m_index(iterator.m_index)
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
        const ValueType& get() const
        {
            assert(!is_done());
            return *(m_data.at(m_index));
        }
        ValueType& get()
        {
            assert(!is_done());
            return *(m_data.at(m_index));
        }
        ValueType& operator*()
        {
            return get();
        }
        const ValueType& operator*() const
        {
            return get();
        }
        ValueType* operator->()
        {
            return &get();
        }
        const ValueType* operator->() const
        {
            return &get();
        }
        Position get_position() const
        {
            assert(!is_done());
            return Position(m_index);
        }
        void update()
        {
            m_max_index = m_data.size();
        }
        operator bool() { return !is_done(); }
    };
    template <typename ValueType, typename DataTypeBase, typename UserDataType>
    class Iterator_Sorted
    {
        DataTypeBase& m_data;
        std::size_t m_index = 0;
        std::vector<std::size_t> m_indices;
        std::function<bool(const T&, const T&, UserDataType)> m_compare;
        // https://stackoverflow.com/a/10581051
        std::vector<std::size_t> get_ordered(std::vector<Ptr<T>> const& values) {
            std::vector<std::size_t> indices(values.size());
            std::iota(std::begin(indices), std::end(indices), static_cast<std::size_t>(0));

            std::sort(
                std::begin(indices), std::end(indices),
                [&](std::size_t l, std::size_t r)
                {
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
        Iterator_Sorted(DataTypeBase& data, std::function<bool(const T&, const T&, UserDataType)> compare)
        : m_data(data), m_compare(compare)
        {
            to_begin_update();
        }
        Iterator_Sorted(const Iterator_Sorted<ValueType, DataTypeBase, UserDataType>& it)
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
        const ValueType& get() const
        {
            assert(!is_done());
            return *(m_data.at(m_indices[m_index]));
        }
        ValueType& get()
        {
            assert(!is_done());
            return *(m_data.at(m_indices[m_index]));
        }
        const ValueType& operator*() const
        {
            return get();
        }
        ValueType& operator*()
        {
            return get();
        }
        const ValueType* operator->() const
        {
            return &get();
        }
        ValueType* operator->()
        {
            return &get();
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
        operator bool() { return !is_done(); }
    };
    template <typename UserDataType>
    bool compare_default(const T& lhs, const T& rhs, const UserDataType)
    {
        return lhs < rhs; // helper
    }

public:
    explicit Vector_Sortable() : m_data() { }
    explicit Vector_Sortable(std::size_t size) : m_data(size, new T()) { }
    explicit Vector_Sortable(std::initializer_list<T> initializer) : m_data(initializer) { }
    ~Vector_Sortable() = default;

    // Usage: for (auto it = vector_sortable.begin(); it; it.next()) { it->something(); }
    // do-while LOOP WILL CRASH ON 0 ELEMENTS.
    Iterator<T, DataTypeBase> begin()
    {
        return Iterator<T, DataTypeBase>(m_data);
    }
    Iterator<const T, const DataTypeBase> cbegin() const
    {
        return Iterator<const T, const DataTypeBase>(m_data);
    }
    
    // Well... I need to initialize it somehow:
    template <typename UserDataType = void*>
    Iterator_Sorted<T, DataTypeBase, const UserDataType>
        sorted_begin(std::function<bool(const T&, const T&, const UserDataType)> compare = compare_default<UserDataType>)
    {
        return Iterator_Sorted<T, DataTypeBase, const UserDataType>(m_data, compare);
    }
    template <typename UserDataType = void*>
    Iterator_Sorted<const T, const DataTypeBase, const UserDataType>
        csorted_begin(std::function<bool(const T&, const T&, const UserDataType)> compare = compare_default<UserDataType>) const
    {
        return Iterator_Sorted<const T, const DataTypeBase, const UserDataType>(m_data, compare);
    }

    DataTypeBase& ref_data()
    {
        return m_data;
    }
    const DataTypeBase& cref_data() const
    {
        return m_data;
    }
    Position push_back(Ptr<T> value)
    {
        m_data.push_back(std::move(value));
        return Position(m_data.size() - 1);
    }
    T& ref(const Position& position)
    {
        IM_ASSERT(is_pos_valid(position));
        return *m_data[position.get()];
    }
    const T& cref(const Position& position) const
    {
        IM_ASSERT(is_pos_valid(position));
        return *m_data[position.get()];
    }
    bool is_pos_valid(const Position& position) const
    {
        return position.get() < m_data.size();
    }
    std::size_t size() const
    {
        return m_data.size();
    }
};

template <typename T>
using Position = typename Vector_Sortable<T>::Position;