/*
 * bootSector.h
 *
 *      Author: alextaxiera
 */

#ifndef BOOTSECTOR_H_
#define BOOTSECTOR_H_

struct bootSector {
	int bytesPerSector;
	int sectorsPerCluster;
	int numFats;
	int numReservedSectors;
	int numRoot;
	int numSectors;
	int sectorsPerFat;
	int sectorsPerTrack;
	int numHeads;
	char bootSig;
	char* volumeId;
	char* systemType;
	char* volumeLabel;
};

#endif /* BOOTSECTOR_H_ */
