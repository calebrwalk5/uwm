#pragma once

#include <utility>
#include <fstream>

#include "basic_log.hpp"

/*#define make_guard(i, o) \
    auto CONCAT(_anonymous, __COUNTER__) = _make_guard(i, o)*/

#define make_guard \
    auto CONCAT(_anonymous, __COUNTER__) = _make_guard

template <typename fun_in, typename fun_out>
class Guard {
public:
    Guard(fun_in&& i_, fun_out&& o_)
        : moved{ false }
        , i{ i_ }
        , o{ o_ }
    {
        if(!file.is_open()) {
            file.open("log.log");
        }
        ++cpt;
        log(file, __PRETTY_FUNCTION__, " -> ", cpt, " instances", "\n");
        i();
    }
    Guard(Guard const&) = delete;
    Guard(Guard&& other)
        : moved{ false }
        , i{ std::move(other.i) }
        , o{ std::move(other.o) }
    {
        other.moved = true;
        log(file, __PRETTY_FUNCTION__, " -> ", cpt, " instances","\n");
    }
    Guard& operator=(Guard&& other)
    {
        moved = false;
        i = std::move(other.i);
        o = std::move(other.o);
        other.moved = true;
    }
    Guard& operator=(Guard const&) = delete;
    ~Guard()
    {
        --cpt;
        if (moved == false) {
            log(file, __PRETTY_FUNCTION__, " -> ", cpt, " instances","\n");
            o();
        } else {
            log(file, "No cleaning : ", __PRETTY_FUNCTION__, " -> ", cpt, " instances","\n");
        }
    }

private:
    static std::ofstream file;
    static unsigned cpt;
    bool moved;
    fun_in i;
    fun_out o;
};
template <typename fun_in, typename fun_out>
std::ofstream Guard<fun_in, fun_out>::file = std::ofstream{ "log.log" };
template <typename fun_in, typename fun_out>
unsigned Guard<fun_in, fun_out>::cpt = 0;

template <typename in, typename out>
constexpr Guard<in, out> _make_guard(in&& i, out&& o)
{
    return Guard<in, out>{ std::forward<in>(i), std::forward<out>(o) };
}
