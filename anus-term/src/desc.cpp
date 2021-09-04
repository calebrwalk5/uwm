#include "desc.hpp"

extern "C" {
#include <pty.h>
#include <sys/time.h>
#include <unistd.h>
}

namespace DESC {
FileDescriptor::FileDescriptor(int desc, bool closeable) noexcept : file_desc{ desc }, is_closeable{ closeable } {
}
FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept
    : file_desc{ other.file_desc },
      is_closeable{ other.is_closeable } {
    other.is_closeable = false;
}
FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    file_desc = other.file_desc;
    is_closeable = other.file_desc;
    other.is_closeable = false;
    return *this;
}
void FileDescriptor::premature_close() noexcept {
    close(file_desc);
    is_closeable = false;
}

int& FileDescriptor::as_int() noexcept {
    return file_desc;
}

FileDescriptor::~FileDescriptor() noexcept {
    if (is_closeable)
        close(file_desc);
}
void set(int desc, Set& set) {
    FD_SET(desc, &(set.set));
}
void select(int desc, Set& read, Set& write, Set& except) {
    ::select(desc + 1, &read.set, &write.set, &except.set, nullptr);
}
void select(int desc, Set& read, Set& write, Set& except, timeval& tv) {
    ::select(desc + 1, &read.set, &write.set, &except.set, &tv);
}
void select(int desc, optional<Set>& read, optional<Set>& write, optional<Set>& except, timeval& tv) {
    ::select(desc + 1, read ? &read->set : nullptr, write ? &write->set : nullptr, except ? &except->set : nullptr, &tv);
}
bool isset(int desc, Set const& set) {
    return FD_ISSET(desc, &set.set);
}
}
