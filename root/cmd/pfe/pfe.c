#include <stdlib.h>
#include <stdio.h>

#include <disk.h>
#include <fat.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("PFE requires a min and max\n");
		return 0;
	}

	int min = atoi(argv[1]);
	int max;

	if (argc < 3) {
		max = min;
	} else {
		max = atoi(argv[2]);
	}

	if (min < 2) {
		printf("Invalid min value\n");
		return 1;
	}

	if (min > max) {
		printf("min cannot exceed max\n");
		return 1;
	}

	unsigned char *fat = NULL;
	fat = fat12_read_table();

	if (fat == NULL) {
		printf("Unable to read FAT12 table\n");
		return 2;
	}

	int i;
	for (i = min; i <= max; ++i) {
		int entry = fat12_get_fat_entry(i, fat);
		printf("Entry %d: %d\n", i, entry);
	}

	free(fat);

    return 0;
}
