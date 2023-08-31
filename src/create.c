#define _GNU_SOURCE

#include "create.h"

#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "container_status.h"
#include "utls.h"

pid_t detach_process() {
    if (setsid() < 0) {
        perror("Failed to create new session");
        _exit(EXIT_FAILURE);
    };

    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork");
        _exit(EXIT_FAILURE);
    }

    return pid;
}

int save_pid(char *path, pid_t pid) {
    AUTO_CLOSE_FILE FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d", pid);

    return 0;
}

int create_container(char *container_id) {
    int container_ready_pipe[2];
    if (pipe(container_ready_pipe) == -1) {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE);
    }

    AUTO_CLOSE int read_fd = container_ready_pipe[0];
    AUTO_CLOSE int write_fd = container_ready_pipe[1];

    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        if (unshare(CLONE_NEWPID) == -1) {
            perror("Failed to unshare");
            exit(EXIT_FAILURE);
        }

        pid_t ret = detach_process();

        if (ret > 0) {
            write(write_fd, &ret, sizeof(ret));
            _exit(EXIT_SUCCESS);
        }

        // Wait for parent process to freeze this process
        usleep(0.1 * 1000 * 1000);  // 0.1 seconds

        // After thawed
        printf("Hello, world!");
        fflush(stdout);

        pid_t current_pid = getpid();
        save_pid("./pid", current_pid);

        exit(EXIT_SUCCESS);
    }

    // In parent process
    pid_t container_pid;
    TEMP_FAILURE_RETRY(read(read_fd, &container_pid, sizeof(container_pid)));

    freeze_process(container_pid);
    save_container_pid(container_id, container_pid);

    return 0;
}

int freeze_process(pid_t pid) {
    printf("Freezing process %d\n", pid);
    if (kill(pid, SIGSTOP) == -1) {
        perror("Failed to freeze process");
        exit(EXIT_FAILURE);
    }
    return 0;
}
