#pragma once
/* Borland/DOS process.h compatibility for Linux */
#include <cstdarg>
#include <string>
//#include <strings.h> // strcasecmp

#ifndef strcmpi
#define strcmpi(a,b) strcasecmp((a),(b))
#endif

#ifndef P_WAIT
#define P_WAIT 0
#endif

#include <cstdlib>   // exit, system, malloc/free in some cases
#include <unistd.h>  // getpid (if needed)

/* Some Borland code uses these names */
#ifndef getpid
#define getpid() ((int)::getpid())
#endif
// Very small spawnlp replacement: builds a command string and calls system().
// Good enough to compile; we can improve behavior later.
static inline int spawnlp(int /*mode*/, const char *file, const char *arg0, ...)
{
    (void)arg0;
    std::string cmd = file ? file : "";

    va_list ap;
    va_start(ap, arg0);
    for (;;)
    {
        const char *s = va_arg(ap, const char*);
        if (!s) break;
        cmd += " ";
        cmd += s;
    }
    va_end(ap);

    return std::system(cmd.c_str());
}
