#include "container_status.h"

#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utls.h"

char **get_existing_container_ids() {
    // TODO: Make directory if it doesn't exist
    AUTO_CLOSE_DIR DIR *ds = opendir(CONTAINERS_PATH);
    if (ds == NULL) {
        perror("Failed to open containers directory");
        exit(EXIT_FAILURE);
    }

    char **container_ids = malloc(sizeof(char *) * 1024);

    struct dirent *entry;
    int i = 0;
    while ((entry = readdir(ds))) {
        // Exclude current directory and parent directory, and hidden files
        if (entry->d_name[0] == '.') {
            continue;
        }

        struct stat sb;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", CONTAINERS_PATH,
                 entry->d_name);
        if (stat(fullpath, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            char *container_id = malloc(sizeof(char) * 1024);
            snprintf(container_id, 1024, "%s", entry->d_name);
            container_ids[i] = container_id;
            i++;
        }
    }
    container_ids[i] = NULL;

    return container_ids;
}

int validate_container_id(char *container_id) {
    char **container_ids __attribute__((cleanup(free_container_ids))) =
        get_existing_container_ids();

    for (int i = 0; container_ids[i] != NULL; i++) {
        if (strcmp(container_ids[i], container_id) == 0) {
            fprintf(stderr,
                    "Container ID is not valid: Container ID must be unique.");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int create_container_dir(char *container_id) {
    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", CONTAINERS_PATH,
             container_id);

    if (mkdir(fullpath, 0755) == -1) {
        perror("Failed to create container directory");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int save_container_pid(char *container_id, pid_t pid) {
    char container_pid_path[1024];
    snprintf(container_pid_path, sizeof(container_pid_path), "%s/%s/pid",
             CONTAINERS_PATH, container_id);

    AUTO_CLOSE_FILE FILE *container_pid_file = fopen(container_pid_path, "w");
    if (container_pid_file == NULL) {
        perror("Failed to open container pid file");
        exit(EXIT_FAILURE);
    }

    fprintf(container_pid_file, "%d", pid);

    return 0;
}

int get_container_pid_from_id(char *container_id) {
    char container_pid_path[1024];
    snprintf(container_pid_path, sizeof(container_pid_path), "%s/%s/pid",
             CONTAINERS_PATH, container_id);

    AUTO_CLOSE_FILE FILE *container_pid_file = fopen(container_pid_path, "r");
    if (container_pid_file == NULL) {
        perror("Failed to open container pid file");
        exit(EXIT_FAILURE);
    }

    char container_pid_str[1024];
    if (fgets(container_pid_str, sizeof(container_pid_str),
              container_pid_file) == NULL) {
        perror("Failed to read container pid file");
        exit(EXIT_FAILURE);
    }

    pid_t container_pid = atoi(container_pid_str);

    return container_pid;
}
