#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "container_status.h"
#include "create.h"
#include "start.h"
#include "utls.h"

int create_entry(int argc, char *argv[]) {
    int opt;

    char *container_id = NULL;
    char *console_socket_path = NULL;

    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
            case 'c':
                console_socket_path = optarg;
                break;
            default:
                fprintf(stderr,
                        "Usage: %s container_id [-c console_socket_path]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    container_id = argv[optind];

    printf("id: %s\n", argv[optind]);
    printf("console_socket_path: %s\n", console_socket_path);

    validate_container_id(container_id);

    create_container_dir(container_id);

    create_container(container_id);

    return 0;
}

int start_entry(int argc, char *argv[]) {
    char *container_id = argv[1];

    pid_t container_pid = get_container_pid_from_id(container_id);
    printf("container_pid: %d\n", container_pid);

    thaw_process(container_pid);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [<args>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *command = argv[1];

    if (strcmp(command, "create") == 0) {
        create_entry(argc - 1, argv + 1);
    } else if (strcmp(command, "start") == 0) {
        start_entry(argc - 1, argv + 1);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
