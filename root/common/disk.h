#ifndef DISK_H
#define DISK_H

#include <stdbool.h>
#include <stdio.h>

void init_mount();

bool mount( const char * disk );
bool umount();

bool is_mounted();

FILE * get_disk();

int get_total_sectors();
int get_bytes_per_sector();

int read_sector ( int sector_number, unsigned char *buffer );

#endif // DISK_H
