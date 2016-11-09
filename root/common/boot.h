#ifndef BOOTSECTOR_H
#define BOOTSECTOR_H

#define BYTES_IN_BOOT_SECTOR 		62
#define BOOT_SEC_VOLUME_LABEL_LEN 	11
#define BOOT_SEC_FS_TYPE_LEN 		8

typedef struct bootSect {
	int 	bytes_per_sector;
	int 	sectors_per_cluster;
	int 	reserved_sectors;
	int 	fat_count;
	int 	root_entries;
	int 	sector_count;
	int 	sectors_per_fat;
	int 	sectors_per_track;
	int 	head_count;
	int 	boot_signature;
	int 	volume_id;
	char 	volume_label[BOOT_SEC_VOLUME_LABEL_LEN + 1];
	char 	fs_type[BOOT_SEC_FS_TYPE_LEN + 1];
} bootSector;

void readBootSector( bootSector *bootSec );

#endif // BOOTSECTOR_H
