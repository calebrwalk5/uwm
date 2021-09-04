#pragma once

#include <experimental/optional>

extern "C" {
#include <sys/select.h>
}

using std::experimental::optional;
namespace DESC {

class FileDescriptor {
public:
    FileDescriptor(int desc, bool closeable = true) noexcept;

    FileDescriptor(FileDescriptor const&) = delete;
    FileDescriptor(FileDescriptor&& other) noexcept;

    FileDescriptor& operator=(FileDescriptor const&) = delete;
    FileDescriptor& operator=(FileDescriptor&& other) noexcept;

    void premature_close() noexcept;

    int& as_int() noexcept;

    ~FileDescriptor() noexcept;

private:
    int file_desc;
    bool is_closeable;
};

class Set {
public:
    Set()
    {
        FD_ZERO(&set);
    }

private:
    friend void set(int desc, Set& set);
    friend void select(int desc, Set& read, Set& write, Set& except);
    friend void select(int desc, optional<Set>& read, optional<Set>& write, optional<Set>& except, timeval& tv);
    friend void select(int desc, Set& read, Set& write, Set& except, timeval& tv);
    friend bool isset(int desc, Set const& set);
    fd_set set;
};
void set(int desc, Set& set);
void select(int desc, Set& read, Set& write, Set& except);
void select(int desc, Set& read, Set& write, Set& except, timeval& tv);
void select(int desc, optional<Set>& read, optional<Set>& write, optional<Set>& except, timeval& tv);
bool isset(int desc, Set const& set);
}

struct stdio_desc_set {
    DESC::Set read;
    DESC::Set write;
    DESC::Set except;
};
