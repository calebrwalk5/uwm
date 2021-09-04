#pragma once

#include <iostream>

template <typename T>
void log_helper(std::ostream& out, T arg) {
    out << arg << std::flush;
}

template <typename T, typename... U>
void log_helper(std::ostream& out, T arg, U... args) {
    out << arg << " ";
    log_helper(out, args...);
}

template <typename... T>
#ifndef NDEBUG
void log(std::ostream& out, T... args) {
    log_helper(out, args...);
#else
void log(std::ostream&, T...) {
#endif
#ifndef NDEBUG
#endif
}
