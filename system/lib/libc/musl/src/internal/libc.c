#include "libc.h"

struct __libc __libc;

size_t __hwcap;
// Modified by Benoit Baudaux 31/05/2023
#ifndef __EMSCRIPTEN__EXA
char *__progname=0, *__progname_full=0;

weak_alias(__progname, program_invocation_short_name);
weak_alias(__progname_full, program_invocation_name);
#endif
