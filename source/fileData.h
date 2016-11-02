/*
 * fileData.h
 *
 *      Author: alextaxiera
 */

#ifndef FILEDATA_H_
#define FILEDATA_H_

struct fileData{
	char* filename;
	char* ext;
	int attr;
	int firstLogicalCluster;
	int fileSize;
};

#endif /* FILEDATA_H_ */
