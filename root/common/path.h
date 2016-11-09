#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

#define MAX_PATH 128

char *clean_path(const char * path);

char *get_cwd();
bool set_cwd(const char *cwd);
int getCluster(const char *cwd);

#endif // PATH_H
