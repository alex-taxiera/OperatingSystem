#include "fatSupport.h"

extern FILE* FILE_SYSTEM_ID;
extern int BYTES_PER_SECTOR;

int read_sector(int sector_number, unsigned char * buffer)
{
	int bytes_read;

	if (fseek(FILE_SYSTEM_ID,
		(long)sector_number * (long)BYTES_PER_SECTOR, SEEK_SET) != 0)
	{
		printf("Error accessing sector %d\n", sector_number);
		return -1;
	}

	bytes_read = fread(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

	if (bytes_read != BYTES_PER_SECTOR)
	{
		printf("Error reading sector %d\n", sector_number);
		return -1;
	}

	return bytes_read;
}

int write_sector(int sector_number, unsigned char * buffer)
{
	int bytes_written;

	if (fseek(FILE_SYSTEM_ID,
		(long)sector_number * (long)BYTES_PER_SECTOR, SEEK_SET) != 0)
	{
		printf("Error accessing sector %d\n", sector_number);
		return -1;
	}

	bytes_written = fwrite(buffer,
		sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

	if (bytes_written != BYTES_PER_SECTOR)
	{
		printf("Error reading sector %d\n", sector_number);
		return -1;
	}

	return bytes_written;
}

unsigned int get_fat_entry(int fat_entry_number, unsigned char * fat)
{
	int offset;
	int uv, wx, yz;

	offset = 3 * fat_entry_number / 2;

	// Two FAT12 entries are stored into three bytes;
	// if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw

	// odd fat entry number, return yzw
	if (fat_entry_number & 0x0001)
	{
		wx = (int)fat[offset];
		yz = (int)fat[offset + 1];
		return ((yz << 4) | ((wx & 0x00f0) >> 4));
	}
	// even fat entry number, return xuv
	else
	{
		uv = (int)fat[offset];
		wx = (int)fat[offset + 1];
		return (((wx & 0x000f) << 8) | uv);
	}
}

void set_fat_entry(int fat_entry_number, int value, unsigned char * fat)
{
	int offset;
	int uv, wx, yz, a, b, c;

	offset = 3 * fat_entry_number / 2;

	// Two FAT12 entries are stored into three bytes;
	// if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw
	// Let 0a,bc denote the fat_entry_number, written as two bytes (high and
	// low, respectively)

	a = value & 0x0f00;
	b = value & 0x00f0;
	c = value & 0x000f;

	// odd fat entry number, change yzw to abc, i.e.,
	if (fat_entry_number & 0x0001)
	{
		// wx = cx;
		fat[offset] = (unsigned char)((c << 4) | (fat[offset] & 0x000f));
		// yz = ab;
		fat[offset + 1] = (unsigned char)((a >> 4) | (b >> 4));
	}
	// even fat entry number, change xuv to abc, i.e.,
	else
	{
		// uv = bc;
		fat[offset] = (unsigned char)(b | c);
		// wx = wa;
		fat[offset + 1] = (unsigned char)((fat[offset + 1] &
			0x00f0) | (a >> 8));
	}
}
