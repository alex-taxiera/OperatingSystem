#include "shm.h"
#include <stdio.h>
#include <memory.h>
// #include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "disk.h"
#define SHMKEY 7874
#define SHMSIZE sizeof(sharedinfo)

bool createShMem(void **ptr) 
{
	int shMemid = 0;
	void *shMemPtr = NULL;

	if ((shMemid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) < 0 ) 
	{
		fprintf(stderr, "Error creating shared memory segment\n");
		return false;
	}

	if ((shMemPtr = shmat(shMemid, NULL, 0)) == NULL) 
	{
		fprintf(stderr, "Error including shared memory address space\n");
		return false;
	}

	if (ptr)
		*ptr = shMemPtr;
	return true;
}

bool lock_shm(void ** ptr) 
{
	int shMemid = 0;
	void *shMemPtr = NULL;

	if ((shMemid = shmget(SHMKEY, SHMSIZE, 0444)) < 0 ) 
		return false;

	if ((shMemPtr = shmat(shMemid, NULL, 0)) == NULL) 
	{
		fprintf(stderr, "Error including shared memory address space.\n");
		return false;
	}

	if (ptr) 
		*ptr = shMemPtr;
	return true;
}

bool release_shm(void * ptr) 
{
	if (shmdt(ptr) < 0) 
	{
		fprintf(stderr, "Error deleting shared memory\n");
		return false;
	}
	
	return true;
}
