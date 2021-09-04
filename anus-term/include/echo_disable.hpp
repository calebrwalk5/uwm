#pragma once

#include <termios.h>
#include <unistd.h>

/// Struct and macro trick to disable echo from stdin and enable it automatically at the end
/// Uses RAII to ensure re-enableing echo from stdin
/// Uses macro to create anonymous variable

#define _CONCAT_(x, y) x##y
#define CONCAT(x, y) _CONCAT_(x, y)

#ifndef __COUNTER__
#define __COUNTER__ __LINE__
#endif

struct echo_disabler {
    echo_disabler()
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ECHO | ECHONL | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
    }
    ~echo_disabler()
    {
        oldt.c_lflag |= ECHO | ECHONL | ICANON;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
    }

private:
    termios oldt;
    termios newt;
};

#define disable_stdin_echo \
    echo_disabler CONCAT(_anonymous, __COUNTER__) {}
