#include "boot.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"
#include "disk.h"

void readBootSector( bootSector *bSect ) 
{
	unsigned char *boot = malloc(BYTES_IN_BOOT_SECTOR * sizeof(unsigned char));
	FILE *fp = get_disk();
	if (fp && fseek(fp, 0 * (long) BYTES_IN_BOOT_SECTOR, SEEK_SET) != 0) 
	{
	   	printf("Could not access boot sector\n");
	  	return;
	}

	int bytes_read = fread(boot, sizeof(char), BYTES_IN_BOOT_SECTOR, fp);
	if (bytes_read != BYTES_IN_BOOT_SECTOR) 
	{
	  	printf("Error: Could not read boot sector\n");
	  	return;
	}

	if (bSect != NULL) 
	{
	    memset(bSect, 0, sizeof(bootSector));
		bSect->bytes_per_sector 	= get_word(boot, 11);
		bSect->sectors_per_cluster	= get_byte(boot, 13);
		bSect->reserved_sectors 	= get_word(boot, 14);
		bSect->fat_count 			= get_byte(boot, 16);
		bSect->root_entries		    = get_word(boot, 17);
		bSect->sector_count		    = get_word(boot, 19);
		bSect->sectors_per_fat		= get_word(boot, 22);
		bSect->sectors_per_track	= get_word(boot, 24);
		bSect->head_count			= get_word(boot, 26);
		bSect->boot_signature		= get_byte(boot, 38);
		bSect->volume_id			= get_dword(boot, 39);
		
        
		memcpy(bSect->volume_label, boot + 43, BOOT_SEC_VOLUME_LABEL_LEN);
		memcpy(bSect->fs_type, boot + 54, BOOT_SEC_FS_TYPE_LEN);
	}
}
