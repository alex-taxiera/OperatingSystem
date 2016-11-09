/*
 * pbs.c
 *
 *      Author: alextaxiera
 */


#include "shell.h"

struct bootSector readBootSector() {
	struct bootSector bootSector;
	char* boot;
	// Set it to this only to read the boot sector
	BYTES_PER_SECTOR = BYTES_TO_READ_IN_BOOT_SECTOR;
	// Then reset it per the value in the boot sector



	boot = (char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	if (read_sector(0, boot) == -1)
	{
		printf("Something has gone wrong -- could not read the boot sector\n");
	}



	bootSector.volumeLabel = malloc(11 * sizeof(char));
	bootSector.systemType = malloc(8 * sizeof(char));
	bootSector.volumeId = malloc(4 * sizeof(char));
	bootSector.bytesPerSector = getBytes(boot, 11);
	bootSector.sectorsPerCluster = (int) boot[13];
	bootSector.numFats = (int) boot[16];
	bootSector.numReservedSectors = getBytes(boot, 14);
	bootSector.numRoot = getBytes(boot, 17);
	bootSector.numSectors = getBytes(boot, 19);
	bootSector.sectorsPerFat = getBytes(boot, 22);
	bootSector.sectorsPerTrack = getBytes(boot, 24);
	bootSector.numHeads = getBytes(boot, 26);
	bootSector.bootSig = boot[38];
	for (int i = 0; i < 4; i++) {
		bootSector.volumeId[i] = boot[39 + i];
	}
	for (int i = 0; i < 8; i++) {
		bootSector.systemType[i] = boot[54 + i];
	}
	for (int i = 0; i < 11; i++) {
		bootSector.volumeLabel[i] = boot[43 + i];
	}
	BYTES_PER_SECTOR = bootSector.bytesPerSector;
	return bootSector;
}

void pbs(struct bootSector bootSector) {
	printf("Bytes per sector			=%d\n", bootSector.bytesPerSector);
	printf("Sectors per cluster			=%d\n", bootSector.sectorsPerCluster);
	printf("Number of FATs				=%d\n", bootSector.numFats);
	printf("Number of Reserved Sectors		=%d\n", bootSector.numReservedSectors);
	printf("Number of root entries 			=%d\n", bootSector.numRoot);
	printf("Total sector count			=%d\n", bootSector.numSectors);
	printf("Sectors per FAT				=%d\n", bootSector.sectorsPerFat);
	printf("Sectors per track			=%d\n", bootSector.sectorsPerTrack);
	printf("Number of heads				=%d\n", bootSector.numHeads);
	printf("Boot signature (in hex)			=0x%2x\n", bootSector.bootSig);
	printf("Volume ID (in hex)			=0x8%2x\n", bootSector.volumeId);
	printf("Volume Label				=%s\n", bootSector.volumeLabel);
	printf("File System Type			=%s\n", bootSector.systemType);
}
