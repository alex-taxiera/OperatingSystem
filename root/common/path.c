#include "path.h"

#include <string.h>
#include <stdlib.h>
#include "shm.h"
#include "fat.h"
#include "functions.h"

char *clean_path( const char *raw_path )
{
	int parts_ind = 0;
	char ** steps = malloc(sizeof(char*) *MAX_PATH);
	memset(steps, 0, sizeof(char*) *MAX_PATH);

	char * path = strdup(raw_path);
	char * pch = strtok(path, "/");
	while (pch != NULL) 
	{
		if (parts_ind > 0 && strcmp(pch, "..") == 0) 
		{
			--parts_ind;
			free(steps[parts_ind]);
			steps[parts_ind] = NULL;
		}
		
		else if (strcmp(pch, ".") != 0) 
		{
			steps[parts_ind] = strdup(pch);
			++parts_ind;
		}
		
		pch = strtok(NULL, "/");
	}
	
	free(path);
	int new_path_ind = 0;
	char * new_path = malloc(sizeof(char) * MAX_PATH);

	if (raw_path[0] == '/') 
	{
		new_path[0] = '/';
		++new_path_ind;
	}

	while (parts_ind > 0) 
	{
		--parts_ind;
		strcpy(new_path + new_path_ind, steps[parts_ind]);
		new_path_ind += strlen(steps[parts_ind]);

		if (parts_ind != 1) 
		{
			new_path[new_path_ind] = '/';
			++new_path_ind;
		}

		free(steps[parts_ind]);
		steps[parts_ind] = NULL;
	}
	
	free(steps);
	new_path[new_path_ind] = '\0';
	return new_path;
}

char *get_cwd() 
{
	void * shm_ptr;
	if (!lock_shm(&shm_ptr)) 
		return false;

	sharedinfo info;
	memcpy(&info, shm_ptr, sizeof(sharedinfo));

	if (!release_shm(shm_ptr)) 
		return false;
	return strdup(info.path);
}

bool set_cwd( const char *new_cwd ) 
{
	int cluster;
	char path[MAX_PATH];
	if (new_cwd[0] == '/') // absolute path
		strcpy(path, new_cwd);
	
	else // relative path
	{ 
		char * cwd = get_cwd();
		int cwd_len = strlen(cwd);
		strcpy(path, cwd);
		free(cwd);

		int path_start;
		if (cwd_len == 1)
			path_start = 1;
		
		else 
		{
			path[cwd_len] = '/';
			path_start = cwd_len + 1;
		}
		
		strcpy(path + path_start, new_cwd);
	}

	char * tmp_path = clean_path(path);
	strcpy(path, tmp_path);
	free(tmp_path);

	strtoupper(path);
	cluster = getCluster(path);

	if (cluster == -1)
		return false;

	void *shm_ptr;
	if (!lock_shm(&shm_ptr))
		return false;

	sharedinfo info;
	memcpy(&info, shm_ptr, sizeof(sharedinfo));
	strcpy(info.path, path);
	memcpy(shm_ptr, &info, sizeof(sharedinfo));

	if (!release_shm(shm_ptr))
		return false;
	return true;
}

int getCluster(const char *cwd) 
{
	int cluster = 0;
	char *path;
	if (cwd == NULL) 
		path = get_cwd();
		
	else 
		path = strdup(cwd);

	int i, count;
	bool found;
	fat12_file_entry_t *files;

	char *pch = strtok(path, "/");
	while (pch != NULL) 
	{
		if (cluster == 0 && strcmp(pch, ".") == 0) 
			found = true;
		
		else 
		{
			files = fat12_get_file_entries(cluster, &count);
			found = false;
			for (i = 0; i < count; ++i) 
			{
				if ( ! fat12_is_directory(&files[i]) ) 
					continue;

				if (strcmp(pch, files[i].full_filename) == 0) 
				{
					cluster = files[i].flc;
					found = true;
					break;
				}
			}
			
			free(files);
		}

		if ( !found ) 
		{
			cluster = -1;
			break;
		}
		
		pch = strtok(NULL, "/");
	}

	free(path);
	return cluster;
}
