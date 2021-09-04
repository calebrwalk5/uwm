#pragma once

#include <string>
#include <experimental/optional>
#include <utility>

#include <pty.h>

#include <cstring>

#include "desc.hpp"

namespace pty {
/// openty wrapper c++ wrapper, takes reference instead of ptr and throw if fail
// don't work with std::string because std::string::c_str() returns a const char* and not char*
// TODO : replace char* by std::unique_ptr
std::string _concat_string(char* r, char* l);
std::string _concat_string(const char* r, char* l);
void openpty(int& master, int& slave);
void openpty(int& master, int& slave, char* name);
void openpty(int& master, int& slave, termios& termp);
void openpty(int& master, int& slave, winsize& winp);
void openpty(int& master, int& slave, char* name, termios& termp);
void openpty(int& master, int& slave, char* name, termios& termp, winsize& winp);
void openpty(int& master, int& slave, char* name, winsize& winp);
void openpty(int& master, int& slave, termios& termp, winsize& winp);


optional<std::pair<DESC::FileDescriptor, pid_t>> fork_term();
}
