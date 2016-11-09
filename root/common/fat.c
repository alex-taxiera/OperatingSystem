#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "functions.h"
#include "boot.h"
#include "disk.h"

bool fat12_check_mask( const fat12_file_entry_t * entry, int mask ) 
{ 
    return (entry == NULL ? false : (entry->attributes & mask)); 
}

bool fat12_is_directory( const fat12_file_entry_t *entry ) 
{ 
    return fat12_check_mask(entry, FAT12_ATTR_DIRECTORY_MASK); 
}

char *fat12_filename(char *filename) 
{
	char * fat_filename = malloc(FAT12_FILENAME_LEN + 1);
	memset(fat_filename, ' ', FAT12_FILENAME_LEN);
	fat_filename[FAT12_FILENAME_LEN] = '\0';

	char * pch = strchr(filename, '.');
	if (pch == NULL) 
		memcpy(fat_filename, filename, MIN(strlen(filename), FAT12_FILENAME_LEN));
 
	else 
		memcpy(fat_filename, filename, MIN(pch - filename, FAT12_FILENAME_LEN));

	return fat_filename;
}

char *fat12_extension(char *filename) 
{
	char * fat_extension = malloc(FAT12_EXTENSION_LEN + 1);
	memset(fat_extension, ' ', FAT12_EXTENSION_LEN);
	fat_extension[FAT12_EXTENSION_LEN] = '\0';

	char * pch = strchr(filename, '.');
	if (pch != NULL) 
		memcpy(fat_extension, pch + 1, MIN(strlen(pch + 1), FAT12_EXTENSION_LEN));

	return fat_extension;
}

unsigned char *fat12_read_table() 
{
	bootSector bSect;
	readBootSector(&bSect);
	
	unsigned char *fat = NULL;
	int len = bSect.fat_count * bSect.bytes_per_sector;
	fat = malloc(len);

	int i;
	for (i = 0; i < bSect.fat_count; ++i)
		read_sector(i + 1, fat + (i * bSect.bytes_per_sector));

	return fat;
}

int fat12_get_fat_entry(int fat_entry_number, const unsigned char *fat) 
{
   int offset;
   int uv, wx, yz;

   offset = 3 * fat_entry_number / 2;
   
   if (fat_entry_number & 0x0001)
   {
      wx = (int) fat[offset];
      yz = (int) fat[offset + 1];
      return ( (yz << 4)  |  ( (wx & 0x00f0) >> 4));
   }
   
   else
   {
      uv = (int) fat[offset];
      wx = (int) fat[offset + 1];
      return ( ((wx & 0x000f) << 8)  |  uv );
   }
}

void fat12_set_fat_entry(int fat_entry_number, int value, unsigned char *fat) 
{
   int offset;
   int a, b, c;

   offset = 3 * fat_entry_number / 2;

   a = value & 0x0f00;
   b = value & 0x00f0;
   c = value & 0x000f;

   if (fat_entry_number & 0x0001)
   {
      fat[offset] = (unsigned char) ((c << 4)  |  (fat[offset] & 0x000f));
      fat[offset + 1] = (unsigned char) ((a >> 4)  |  (b >> 4));
   }
   
   else
   {
      fat[offset] = (unsigned char) (b | c);
      fat[offset + 1] = (unsigned char) ((fat[offset + 1]  & 0x00f0)  |  (a >> 8));
   }
}

void fat12_read_file_entry( const unsigned char *sector, int offset, fat12_file_entry_t *entry ) 
{
	memcpy(entry->filename,  sector + offset,     FAT12_FILENAME_LEN);
	memcpy(entry->extension, sector + 8 + offset, FAT12_EXTENSION_LEN);

	int j;
	for (j = FAT12_FILENAME_LEN; j > 0; --j) 
	{
		if (entry->filename[j] == ' ' && entry->filename[j - 1] != ' ') 
		{
			entry->filename[j] = '\0';
			break;
		}
	}
	
	for (j = FAT12_EXTENSION_LEN; j > 0; --j) 
	{
		if (entry->extension[j] == ' ' && entry->extension[j - 1] != ' ') 
		{
			entry->extension[j] = '\0';
			break;
		}
	}
	
	if (entry->extension[0] == ' ')
		entry->extension[0] = '\0';

	entry->attributes = get_byte (sector, 11 + offset);
	entry->flc = get_word (sector, 26 + offset);
	entry->file_size = get_dword(sector, 28 + offset);

	strcpy(entry->full_filename, entry->filename);
	if ( ! fat12_is_directory(entry) && strlen(entry->extension) != 0) 
	{
		strcat(entry->full_filename, ".");
		strcat(entry->full_filename, entry->extension);
	}
}

void fat12_write_file_entry( unsigned char *sector, int offset, const fat12_file_entry_t * entry ) 
{
	memcpy(sector + offset,     entry->filename,  MIN(strlen(entry->filename),  FAT12_FILENAME_LEN));
	memcpy(sector + 8 + offset, entry->extension, MIN(strlen(entry->extension), FAT12_EXTENSION_LEN));

	set_byte (sector, 11 + offset, entry->attributes);
	set_word (sector, 26 + offset, entry->flc);
	set_dword(sector, 28 + offset, entry->file_size);
}

fat12_file_entry_t *fat12_get_file_entries( int cluster, int *count ) 
{
	unsigned char *fat = fat12_read_table();
	int bytes_per_sec = get_bytes_per_sector();

	if (cluster == 0) 
		cluster = FAT12_ROOT_OFFSET;
		
	else 
		cluster += FAT12_DATA_OFFSET;

	int sector_count = 1;
	unsigned char *sectors = malloc(bytes_per_sec *sector_count);
	read_sector(cluster, sectors);
	fat12_file_entry_t large_list[128];

	int file_count = 0;
	int sec, i;
	for (sec = 0; sec < sector_count; ++sec) 
	{
		unsigned char *sector = sectors + sec;
		for (i = 0; i < 16; ++i) 
		{
			int offset = i *FAT12_BYTES_PER_DIR_ENTRY;
			fat12_file_entry_t * file = &large_list[file_count];

			int first_filename_byte = get_byte(sector, offset);
			int file_size = get_dword(sector, 28 + offset);
			int flc = get_word(sector, 26 + offset) + FAT12_DATA_OFFSET;

			if (file_size < 0 || first_filename_byte == 0xE5 || fat12_get_fat_entry(flc, fat) != 0) 
				continue;

			if (first_filename_byte == 0) 
				break;

			fat12_read_file_entry(sector, offset, file);
			++file_count;
		}
	}

	fat12_file_entry_t * list = malloc(file_count * sizeof(fat12_file_entry_t));
	memcpy(list, large_list, file_count * sizeof(fat12_file_entry_t));

	free(sectors);
	free(fat);

	*count = file_count;
	return list;
}
