#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>

#include <initializer_list>

#include "allocator.hpp"
#include "debug.hpp"
#include "isclose.hpp"

/*
 * std::vector is too convoluted for our task.
 *
 */

namespace cmp
{

template <typename T>
class array
{
public:
    explicit array(size_t size)
        : size_(size)
        , data_(reinterpret_cast<T*>(allocator::allocate(sizeof(T) * size_)))
    {
    }

    array()
        : size_(0)
        , data_(nullptr)
    {
    }

    array(const array& other)
        : size_(other.size_)
        , data_(reinterpret_cast<T*>(allocator::allocate(sizeof(T) * size_)))
    {
        memcpy(data_, other.data_, sizeof(T) * size_);
    }

    array(array&& other)
        : size_(other.size_)
        , data_(other.data_)
    {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    array(std::initializer_list<T> l)
        : size_(l.size())
        , data_(reinterpret_cast<T*>(allocator::allocate(sizeof(T) * size_)))
    {
        auto iter = l.begin();
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = *iter;
            ++iter;
        }
    }

    friend bool operator==(const array& a, const array& b)
    {
        if (a.size() != b.size()) {
            return false;
        }

        for (size_t i = 0; i < a.size(); ++i) {
            if (!isclose(a[i], b[i])) {
                return false;
            }
        }

        return true;
    }

    const T& operator[](size_t i) const
    {
        // Checking for bounds is a programmer responsibility. Only in debug.
        if constexpr (debug()) {
            assert(i < size_ && "Out of bounds!");
        }
        return *(data_ + i);
    }

    T& operator[](size_t i)
    {
        if constexpr (debug()) {
            assert(i < size_ && "Out of bounds!");
        }
        return *(data_ + i);
    }

    size_t size() const
    {
        return size_;
    }

    T* data()
    {
        return data_;
    }

    const T* data() const
    {
        return data_;
    }

    ~array()
    {
        allocator::deallocate(data_);
    }

private:
    size_t size_{0};
    T* data_{nullptr};
};
} // namespace cmp