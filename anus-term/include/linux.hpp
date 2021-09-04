#pragma once

#include <string>
#include <experimental/optional>
#include <functional>
#include <cstring>

extern "C" {
#include <unistd.h>
#include <termios.h>
#include <signal.h>
}

namespace linux {
    enum class special_key : char {
        up     = 72,
        down   = 80,
        left   = 75,
        right  = 77,
        escape = 27,
    }; // Not really usefull

    enum class Error : unsigned {
        eperm   = 1,
        enoent  = 2,
        esrch   = 3,
        eintr   = 4,
        eio     = 5,
        enxio   = 6,
        e2big   = 7,
        enoexec = 8,
        ebadf   = 9,
        echild  = 10,
        eagain  = 11,
        enomem  = 12,
        eacces  = 13,
        efault  = 14,
        enotblk = 15,
        ebusy   = 16,
        eexist  = 17,
        exdev   = 18,
        enodev  = 19,
        enotdir = 20,
        eisdir  = 21,
        einval  = 22,
        enfile  = 23,
        emfile  = 24,
        enotty  = 25,
        etxtbsy = 26,
        efbig   = 27,
        enospc  = 28,
        espipe  = 29,
        erofs   = 30,
        emlink  = 31,
        epipe   = 32,
        edom    = 33,
        erange  = 34,
        ok      = 35
    };

    Error errno_to_enum() {
      static Error errors[] =
        { Error::eperm   , Error::enoent , Error::esrch  , Error::eintr , Error::eio,
          Error::enxio   , Error::e2big  , Error::enoexec, Error::ebadf , Error::echild,
          Error::eagain  , Error::enomem , Error::eacces , Error::efault, Error::enotblk,
          Error::ebusy   , Error::eexist , Error::exdev  , Error::enodev, Error::enotdir,
          Error::eisdir  , Error::einval , Error::enfile , Error::emfile, Error::enotty,
          Error::etxtbsy , Error::efbig  , Error::enospc , Error::espipe, Error::erofs,
          Error::emlink  , Error::epipe  , Error::edom   , Error::erange, Error::ok };
        return errors[errno];
    }

    Error chdir(const std::string& path) {
        if(::chdir(path.c_str()) == -1) {
            return errno_to_enum();
        }
        return Error::ok;
    }
    using std::experimental::optional;
    optional<std::string> getenv(const std::string& env) {
        auto res = ::getenv(env.c_str());
        if(res) return optional<std::string>(std::string(res, std::strlen(res)));
        return optional<std::string>{};
    }
    template<typename... T>
    Error execlp(const std::string& cmd, const std::string& path, const T&... args) {
        const auto ret = ::execlp(cmd.c_str(), path.c_str(), args..., nullptr);
        return errno_to_enum();
    }

    ::termios get_termios(int desc) {
        ::termios t;
        ::tcgetattr(desc, &t);
        return t;
    }

    // TODO : replace C termios by this termios and test
    // FIXME : non copiable but movable (not in assignement) (maybe copiable)
    struct termios {
        private:
            int desc;
            ::termios old;
        public:
        ::termios term;

        tcflag_t& c_iflag;      /* input modes */
        tcflag_t& c_oflag;      /* output modes */
        tcflag_t& c_cflag;      /* control modes */
        tcflag_t& c_lflag;      /* local modes */
        cc_t     (&c_cc)[NCCS];   /* special characters */


        termios(int desc_):
            desc{desc_},
            old{get_termios(desc)},
            term{old},
            c_iflag{old.c_iflag},
            c_oflag{term.c_oflag},
            c_cflag{term.c_cflag},
            c_lflag{term.c_lflag},
            c_cc{term.c_cc}
        {
            tcgetattr(desc, &term);
            tcgetattr(desc, &old); // Copy should not cause issue but to be sure
        }

        ~termios() {
            tcsetattr(desc, TCSAFLUSH, &old);
        }

    };

    void tcsetattr(int desc, int option, termios& t) {
        ::tcsetattr(desc, option, &(t.term));
    }

}
