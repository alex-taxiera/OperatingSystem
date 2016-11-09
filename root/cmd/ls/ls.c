#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <disk.h>
#include <path.h>
#include <fat.h>
#include <functions.h>

void printInfo( fat12_file_entry_t * file ) 
{
	int col = 0;
	char spacingStr[30];
	printf("%s", file->full_filename);
	col += strlen(file->full_filename);

	for (; col < 14; ++col)
		printf(" ");

	if (fat12_is_directory(file)) 
	{
		printf("Dir");
		col += 3;
		for (; col < 36; ++col)
			printf(" ");
	} 
	
	else 
	{
		printf("File");
		col += 4;
		for (; col < 22; ++col)
			printf(" ");

		printf("%d", file->file_size);
		sprintf(spacingStr, "%d", file->file_size);
		col += strlen(spacingStr);

		for (; col < 36; ++col)
			printf(" ");
	}

	printf("%d", file->flc);
	sprintf(spacingStr, "%d", file->flc);
	
	col += strlen(spacingStr);
	for (; col < 42; ++col)
		printf(" ");
}

int main(int argc, char** argv) 
{
	int cluster;
	if (argc < 2) 
		cluster = getCluster(NULL);
		
	else 
	{
		char path[MAX_PATH];
		
		// Absolute path
		if (argv[1][0] == '/') 
			strcpy(path, argv[1]);
		
		// Relative path
		else 
		{ 
			char *cwd = get_cwd();
			int cwd_len = strlen(cwd);
			strcpy(path, cwd);
			free(cwd);

			int path_start;
			if (cwd_len == 1) 
				path_start = 1;
			
			else 
			{
				path[strlen(cwd)] = '/';
				path_start = strlen(cwd) + 1;
			}
			
			strcpy(path + path_start, argv[1]);
		}
		
		strtoupper(path);
		cluster = getCluster(path);
	}

	if (cluster == -1) 
	{
		printf("Invalid path\n");
		return 1;
	}

	int count;
	fat12_file_entry_t * files = fat12_get_file_entries(cluster, &count);
	int i;
	for (i = 0; i < count; ++i)
	{ 
		printInfo(files + i);
		printf("\n");
	}

	free(files);
    return 0;
}
