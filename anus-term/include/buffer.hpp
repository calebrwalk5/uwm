#pragma once

#include <algorithm>

#include <cassert>

template <typename T, std::size_t bufsize>
class Buffer {
public:
    using value_t = T;
    using ptr_t = T*;

    Buffer() noexcept
    {
        std::fill(std::begin(buf), std::end(buf), T{});
    };
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;
    constexpr std::size_t size() noexcept { return bufsize; }
    T& operator[](std::size_t idx) noexcept {
        assert(idx < bufsize);
        return buf[idx];
    }
    T const& operator[](std::size_t idx) const noexcept {
        assert(idx < bufsize);
        return buf[idx];
    }


    class iterator {
    public:
        iterator(Buffer<T, bufsize>& buf_, std::size_t i = 0)
            : idx{ i }
            , buf{ buf_ }
        {
        }
        iterator& operator++() noexcept
        {
            assert(idx < bufsize);
            ++idx;
            return *this;
        }
        iterator& operator--() noexcept
        {
            --idx;
            return *this;
        }
        iterator& operator+(std::size_t i) noexcept
        {
            assert(i + idx < bufsize);
            idx += i;
            return *this;
        }
        iterator& operator-(std::size_t i) noexcept
        {
            assert(idx >= i);
            idx -= i;
            return *this;
        }
        bool operator==(iterator const& it) noexcept
        {
            return (&buf == &(it.buf)) && (idx == it.idx);
        }
        bool operator!=(iterator const& it) noexcept
        {
            return !(it == *this);
        }
        Buffer<T, bufsize>::ptr_t as_ptr() noexcept { return buf.buf + idx; }

        T& operator*() noexcept
        {
            return buf.buf[idx];
        }

    private:
        std::size_t idx;
        Buffer<T, bufsize>& buf;
    };
    iterator begin() noexcept
    {
        return iterator(*this);
    }
    iterator end() noexcept
    {
        return iterator(*this, bufsize);
    }

private:
    T buf[bufsize];
};

// TODO : make method that fill the buffer and change iterator so that the next fill fill empty space
