#include "utls.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void auto_close(int *fd) {
    if (*fd != -1) {
        close(*fd);
    }
}

void auto_close_file(FILE **file) {
    if (*file != NULL) {
        fclose(*file);
    }
}

void auto_close_dir(DIR **ds) {
    if (*ds != NULL) {
        closedir(*ds);
    }
}

void free_container_ids(char ***container_ids) {
    for (int i = 0; (*container_ids)[i] != NULL; i++) {
        free((*container_ids)[i]);
    }

    free(*container_ids);
}
