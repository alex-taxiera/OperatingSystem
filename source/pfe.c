/*
 * pfe.c
 *
 *      Author: alextaxiera
 */

#include "shell.h"

int checkRange(int min, int max)
{
	if(min > max)
	{
		printf("First parameter must be less than second.\n");
		return 1;
	}
	else if(min < 2){
		printf("First parameter must be greater than 2.\n");
		return 1;
	}
	return 0;
}

void pfe(int min, int max, struct bootSector bootSector)
{
	if (checkRange(min, max) == 0)
	{
		char* fatTable = readFatTable(bootSector, 0);

		for (int i = min; i < max; i++)
		{
			int val = get_fat_entry(i, fatTable);
			printf("Entry %d: %0x\n", i, val);
		}
		free(fatTable);
	}
}

char* readFatTable(struct bootSector bootSector, int x)
{
	//reads one entire fat table to giant string
	//approx. size = 4K
	char * fatSector = malloc(BYTES_PER_SECTOR*(sizeof(char)));
	char * fatTable = malloc(bootSector.sectorsPerFat * BYTES_PER_SECTOR*(sizeof(char)));
	int start = 1 + (x* bootSector.sectorsPerFat);
	int end = (x + 1) * bootSector.sectorsPerFat;
	if (x > bootSector.numFats)
	{
		printf("Error: tried to read non-existent FAT table.\n");
		return NULL;
	}
	else{
		for(int i = start; i <= end; i++)
		{
			read_sector(i, fatSector);
			fatTable = strncat(fatTable, fatSector, BYTES_PER_SECTOR);
		}
	}
	free(fatSector);
	return fatTable;
}
