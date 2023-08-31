#ifndef _CONTAINER_STATUS_H_
#define _CONTAINER_STATUS_H_

#define CONTAINERS_PATH "/var/run/rrun/containers"

#include <sys/types.h>

char **get_existing_container_ids();
int validate_container_id(char *container_id);
int create_container_dir(char *container_id);
int save_container_pid(char *container_id, pid_t pid);
int get_container_pid_from_id(char *container_id);

#endif
