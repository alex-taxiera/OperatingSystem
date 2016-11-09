#ifndef FAT_H
#define FAT_H

#include <stdbool.h>

#define FAT12_FILENAME_LEN 				8
#define FAT12_EXTENSION_LEN 			3
#define FAT12_ROOT_LENGTH 				13
#define FAT12_ROOT_OFFSET 				19
#define FAT12_DATA_OFFSET 				31
#define FAT12_END_OF_CLUSTER 			4095
#define FAT12_BYTES_PER_DIR_ENTRY 		32
#define FAT12_RESERVED_SECTOR_COUNT 	33
#define FAT12_ENTRIES_PER_FAT 			384

#define FAT12_ATTR_READ_ONLY_MASK 		0x01
#define FAT12_ATTR_HIDDEN_FILE_MASK 	0x02
#define FAT12_ATTR_SYSTEM_FILE_MASK 	0x04
#define FAT12_ATTR_VOLUME_LABEL_MASK 	0x08
#define FAT12_ATTR_LONG_FILE_NAME_MASK 	0x0F
#define FAT12_ATTR_DIRECTORY_MASK		0x10
#define FAT12_ATTR_ARCHIVE_MASK			0x20

typedef struct fat12_file_entry {
	char 	filename		[FAT12_FILENAME_LEN + 1];
	char 	full_filename	[FAT12_FILENAME_LEN + 1 + FAT12_EXTENSION_LEN + 1];
	char 	extension		[FAT12_EXTENSION_LEN + 1];
	
	int 	attributes;
	int 	flc;
	int 	file_size;
	int 	offset;

} fat12_file_entry_t;

bool fat12_check_mask (const fat12_file_entry_t *entry, int mask);
bool fat12_is_directory	(const fat12_file_entry_t *entry);

char *fat12_filename (char *filename);
char *fat12_extension (char *filename);

unsigned char *fat12_read_table();

int  fat12_get_fat_entry (int fat_entry_number, const unsigned char *fat);
void fat12_set_fat_entry (int fat_entry_number, int value, unsigned char *fat);

void fat12_read_file_entry (const unsigned char *sector, int offset, fat12_file_entry_t *entry);
void fat12_write_file_entry (unsigned char *sector, int offset, const fat12_file_entry_t *entry);

fat12_file_entry_t *fat12_get_file_entries (int cluster, int *count);

#endif // FAT_H
