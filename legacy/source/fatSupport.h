/*
 * fatSupport.h
 *
 *  Created on: Nov 1, 2016
 *      Author: alextaxiera
 */

#ifndef FATSUPPORT_H_
#define FATSUPPORT_H_

int read_sector(int sector_number, char* buffer);
int write_sector(int sector_number, char* buffer);

unsigned int get_fat_entry(int fat_entry_number, char* fat);
void set_fat_entry(int fat_entry_number, int value, char* fat);

// 13 is NOT the correct number -- you fix it!
#define BYTES_TO_READ_IN_BOOT_SECTOR 512

/******************************************************************************
 * You must set these global variables:
 *    FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                      filesystem)
 *    BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 *
 * You may use these support functions (defined in FatSupport.c)
 *    read_sector
 *    write_sector
 *    get_fat_entry
 *    set_fat_entry
 *****************************************************************************/

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

#endif /* FATSUPPORT_H_ */
