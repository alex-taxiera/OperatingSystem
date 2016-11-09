#include <boot.h>
#include <disk.h>

int main(int argc, char** argv) {
    
	bootSector bSect;
	readBootSector(&bSect);

	printf("Bytes per Sector            = %d\n",   bSect.bytes_per_sector);
	printf("Sectors per Cluster         = %d\n",   bSect.sectors_per_cluster);
	printf("Number of FATs              = %d\n",   bSect.fat_count);
	printf("Number of Reserved Sectors  = %d\n",   bSect.reserved_sectors);
	printf("Number of root entries      = %d\n",   bSect.root_entries);
	printf("Total Sector Count          = %d\n",   bSect.sector_count);
	printf("Sectors per FAT             = %d\n",   bSect.sectors_per_fat);
	printf("Sectors per Track           = %d\n",   bSect.sectors_per_track);
	printf("Number of Heads             = %d\n",   bSect.head_count);
	printf("Boot Signature (in hex)     = 0x%X\n", bSect.boot_signature);
	printf("Volume ID (in hex)          = 0x%X\n", bSect.volume_id);
	printf("Volume Label                = %s\n",   bSect.volume_label);
	printf("File System Type            = %s\n",   bSect.fs_type);

    return 0;
}
