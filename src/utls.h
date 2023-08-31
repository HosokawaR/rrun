#ifndef _UTLS_H_
#define _UTLS_H_

#define AUTO_CLOSE __attribute__((cleanup(auto_close)))
#define AUTO_CLOSE_FILE __attribute__((cleanup(auto_close_file)))
#define AUTO_CLOSE_DIR __attribute__((cleanup(auto_close_dir)))

#include <dirent.h>
#include <stdio.h>

void auto_close(int *fd);
void auto_close_file(FILE **file);
void auto_close_dir(DIR **ds);
void free_container_ids(char ***container_ids);

#endif
