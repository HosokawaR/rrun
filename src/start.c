#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "create.h"

int thaw_process(pid_t pid) {
    if (kill(pid, SIGCONT) == -1) {
        perror("Failed to thaw process");
        exit(EXIT_FAILURE);
    }
    return 0;
}
