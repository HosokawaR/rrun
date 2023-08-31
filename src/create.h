#ifndef _CREATE_H_
#define _CREATE_H_

#include <sys/types.h>

int create_container(char *container_id);
int freeze_process(pid_t pid);
int thaw_process(pid_t pid);

#endif
