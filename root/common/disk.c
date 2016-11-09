#include "disk.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "shm.h"
#include "boot.h"
#include "fat.h"

void init_mount() 
{
	void *shMemPtr;
	if (!createShMem(&shMemPtr))
		return;

	sharedinfo info;
	info.fd = 0;
	memset(info.path, 0, MAX_PATH + 1);
	memcpy(shMemPtr, &info, sizeof(sharedinfo));

	if (!release_shm(shMemPtr))
		return;
}

bool mount(const char *disk) 
{
	FILE *fp = fopen(disk, "r+");
	if (fp == NULL)	{
		fprintf(stderr, "Cannot open file %s\n", disk);
		return false;
	}

	void *shMemPtr;
	if (!lock_shm(&shMemPtr)) 
	{
		printf("Disk mount failed\n");
		return false;
	}

	sharedinfo info;
	info.fd = fileno(fp);
	strcpy(info.path, "/");
	memcpy(shMemPtr, &info, sizeof(sharedinfo));

	if (!release_shm(shMemPtr)) 
	{
		printf("Disk mount failed\n");
		return false;
	}
    return true;
}

bool umount() 
{
	void *shMemPtr;
	if (!lock_shm(&shMemPtr)) 
	{
		printf("Disk un-mount failed\n");
		return false;
	}

	sharedinfo info;
	memcpy(&info, shMemPtr, sizeof(sharedinfo));

	int fd = info.fd;

	info.fd = 0;
	memset(info.path, 0, MAX_PATH + 1);
	memcpy(shMemPtr, &info, sizeof(sharedinfo));

	if (!release_shm(shMemPtr)) 
	{
		printf("Disk un-mount failed\n");
		return false;
	}

	if (fd && fcntl(fd, F_GETFD) != -1)
		close(fd);
	return true;
}

bool is_mounted() 
{
	void *shMemPtr;
	if (!lock_shm(&shMemPtr)) 
		return false;

	sharedinfo info;
	memcpy(&info, shMemPtr, sizeof(sharedinfo));

	if (!release_shm(shMemPtr)) 
		return false;
	return info.fd != 0;
}

FILE * get_disk() 
{
	void * shMemPtr;
	if (!lock_shm(&shMemPtr))
		return NULL;

	sharedinfo info;
	memcpy(&info, shMemPtr, sizeof(sharedinfo));
	if (!release_shm(shMemPtr)) {
		return NULL;
	}

	FILE *fp = NULL;
	if (info.fd && fcntl(info.fd, F_GETFD) != -1)
		fp = fdopen(info.fd, "r+");
	return fp;
}

int get_total_sectors() 
{
	bootSector bSect;
	readBootSector(&bSect);

    return bSect.sector_count - FAT12_RESERVED_SECTOR_COUNT;
}

int get_bytes_per_sector() 
{
	bootSector bSect;
	readBootSector(&bSect);

    return bSect.bytes_per_sector;
}

int read_sector(int sector_number, unsigned char *buffer) 
{
   	int bytes_read;
   	int bytes_per_sec = get_bytes_per_sector();

   	FILE * fp = get_disk();

   	if (fp && fseek(fp, (long) sector_number * (long) bytes_per_sec, SEEK_SET) != 0) 
   	{
	   	printf("Could not access sector %d\n", sector_number);
      	return -1;
   	}

   	bytes_read = fread(buffer, sizeof(char), bytes_per_sec, fp);

	if (bytes_read != bytes_per_sec) 
	{
		printf("Could not read sector %d\n", sector_number);
		return -1;
	}
   	return bytes_read;
}
