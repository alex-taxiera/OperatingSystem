#ifndef SHM_H
#define SHM_H

#include <stdbool.h>
#include "path.h"

typedef struct sharedinfo
{
    int fd;
    char path[MAX_PATH + 1];

} sharedinfo;

bool create_shm(void ** ptr);
bool lock_shm(void ** ptr);
bool release_shm();

#endif // SHM_H
