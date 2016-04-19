#ifndef GLOBAL_H
#define GLOBAL_H

#define BOLD "\x1b[1m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

#ifndef CLOCK_MONOTONIC_RAW	//OpenSuse 11.4 uses glibc 2.11 which is missing correct definitions for realtime library, for OpenSuse 12.4 work correct
#define CLOCK_MONOTONIC_RAW 4
#endif

#define MAX_MSG_LEN 1024

#endif
