// C++ includes
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <thread>
#include <fstream>
#include <experimental/optional>

// LINUX includes
#include <cstdlib>
#include <pty.h>
#include <sys/time.h>
#include <sys/signalfd.h>
#include <sys/signal.h>
#include <termios.h>
#include <unistd.h>

#include "echo_disable.hpp"
#include "desc.hpp"
#include "guard.hpp"
#include "pty.hpp"
#include "linux.hpp"
#ifndef NDEBUG
#include "debug.hpp"
#endif

#ifndef ASCII_DEBUG
#define ASCII_DEBUG(val)
#endif

const char *SHELL = "/bin/bash";

// ctrl+c => SIGINT
// ctrl+z => SIGTSTP
// ctrl+\ => SIGQUIT
// ctrl+t => SIGINFO

std::string get_shell_name(std::string const& path) {
    std::string tmp;
    tmp.reserve(path.size());
    for (auto const& c : path) {
        if (c == '/') {
            tmp.clear();
        } else {
            tmp.push_back(c);
        }
    }
    return tmp;
}

using namespace std::literals;

std::ofstream log_in{"./tmp-in.log"};
std::ofstream log_out{"./tmp-out.log"};

pid_t shell_pid;

linux::Error talk_to_shell(int master, stdio_desc_set const& descs) {
  if(!DESC::isset(master, descs.read)) return linux::Error::ok;
    unsigned char buf;
    if(read(master, &buf, 1) == -1) return linux::errno_to_enum();
    if(write(STDOUT_FILENO, &buf, 1) == -1) return linux::errno_to_enum();
    return linux::Error::ok;
}

std::experimental::optional<unsigned char> parse_stdin(unsigned char buf) {
    switch (buf) {
        case 'Q':
            return static_cast<unsigned char>(3);
        default:
            return buf;
    }
    return {};
}

bool talk_to_stdin(int master, stdio_desc_set const& descs) {
    if(!DESC::isset(STDIN_FILENO, descs.read)) return true;
    unsigned char buf;
    switch (read(STDIN_FILENO, &buf, 1)) {
        case -1:
            return false;
            break;
        case 0:
            buf = EOF;
            write(master, &buf, 1);
            return false;
            break;
        default:
            log_in << ASCII_DEBUG(buf);
            if(auto buf_ = parse_stdin(buf)) {
                write(master, &(*buf_), 1);
            }
            return true;
    }
}

bool cont = false;
void forward_signal(int sig) {
    kill(-1*shell_pid, sig);
    cont = true;
}


int main() { // This is where the magic happens
	int sdesc;
	sigset_t mask;
 
	/* We will handle SIGTERM and SIGINT. */
	sigemptyset (&mask);
        for(auto sig : { SIGINT, SIGTSTP, SIGQUIT }) {
            sigaddset (&mask, sig);
        }
	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		perror ("sigprocmask");
		return 1;
	}
	sdesc = signalfd (-1, &mask, 0);
	if (sdesc < 0) {
		perror ("signaldesc");
		return 1;
	}

    linux::termios term{STDIN_FILENO};
    term.c_lflag &= ~(ECHO | ECHONL | ICANON);
    linux::tcsetattr(STDIN_FILENO, TCSAFLUSH, term);

    auto desc = pty::fork_term();

    if (desc) {
        bool run = true;

        auto const& master = desc->first.as_int();
        shell_pid = desc->second;

        while (run) {
            stdio_desc_set io_desc;

            DESC::set(master, io_desc.read);
            DESC::set(STDIN_FILENO, io_desc.read);
            DESC::set(sdesc, io_desc.read);
            DESC::select(master + 1, io_desc.read, io_desc.write, io_desc.except);

            if(cont) {
                cont = false;
                std::cout << "kill(" << shell_pid << ", " << "signal)" << std::endl;
                std::this_thread::sleep_for(2s);
                continue;
            }
       
            run = talk_to_shell(master, io_desc) == linux::Error::ok; // Communication with the linux shell

            // Communication with stdin
            run &= talk_to_stdin(master, io_desc);
            if(DESC::isset(sdesc, io_desc.read)) {
		struct signalfd_siginfo si;
 
		ssize_t res;
 
		res = read (sdesc, &si, sizeof(si));
 
		if (res < 0) {
			perror ("read");
			return 1;
		}
		if (res != sizeof(si)) {
			fprintf (stderr, "Something wrong\n");
			return 1;
		}
 
		if (si.ssi_signo == SIGINT) {
                    unsigned char tmp = 3;
                    printf("Got SIGINT, writing %d\n", tmp);
                    write(master, &tmp, 1);
                    cont = true;
                    continue;
		} else {
			fprintf (stderr, "Got some unhandled signal %d\n", si.ssi_signo);
			return 1;
		}
            }
            cont = false;
        }
        close(sdesc);
    } else {
        const std::string shell_path = SHELL;
        const std::string shell_name = get_shell_name(shell_path);
        if (chdir(getenv("HOME")) == -1)
            return 254;
        if (execlp(shell_name.c_str(), shell_path.c_str(), NULL) == -1) {
            return 253;
        }
    }

    return 0;
}
