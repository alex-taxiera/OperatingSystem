/*
 * cd.c
 *
 *      Author: alextaxiera
 */


#include "shell.h"


char* cd(struct bootSector bootSector,char** args, int numParams, char* currentDirectory)
{
	if(numParams == 1)
	{
		 currentDirectory= "/";
	}
	else if (strcmp(args[1],"/")==0)
	{
		currentDirectory = "/";
	}
	else{
		char* mdir = getAbsolutePath(currentDirectory, args[1]);

		char* tmp = malloc(strlen(mdir));
		strcpy(tmp, mdir);
		struct fileData m_file;
		int success = 0;
		success = findTarget(tmp, bootSector, &m_file);
		if(success == 0)
		{
			printf("Directory %s does not exist.\n", mdir);
			free(tmp);
		}
		else if (m_file.attr != 16){
			printf("%s is not a directory.\n", mdir);
			free(tmp);
		}
		else{
			free(tmp);
			currentDirectory = malloc(strlen(mdir));
			currentDirectory = strcpy(currentDirectory, mdir);
			free(mdir);
		}
	}
	return currentDirectory;
}

/*
 * Checks to see if a given path is absolute or relative.
 * If relative, creates absolute path.
 */
char* getAbsolutePath(char* cwd, char* args)
{
	int msize, len = 0;
	char* targetDirectory = "";
	targetDirectory = (char *) malloc(1);
	if( strncmp(args, "/", 1) != 0)
	{
		msize = strlen(cwd);
		targetDirectory =(char *) malloc(msize);
		strcat(targetDirectory, cwd);
		int len = strlen(cwd);
		//printf("last char: %d, %c\n", len, cwd[len-1]);
		if(len > 0){
			if (cwd[len-1] != '/'){
				msize += 1;
				targetDirectory = (char *) realloc(targetDirectory, msize);
				strcat(targetDirectory, "/");
			}
		}
	}
	msize += strlen(args);
	targetDirectory = (char *) realloc(targetDirectory, msize);
	strcat(targetDirectory, args);
	return targetDirectory;
}

int findTarget(char* abs_path, struct bootSector bootSector, struct fileData* MF)
{

	int found = 0;
	char ** args;
	int numParams = 1;
	args = malloc(numParams * sizeof(char*));
	abs_path = strtok(abs_path, "/");
	args[0] = strtok(abs_path, "/");
	while (args[numParams-1] != NULL) {
		//find number of parameters
		numParams += 1;
		args = realloc(args, numParams * sizeof(char*));
		args[numParams - 1] = strtok(NULL, "/");
	}
	//printf("numParams = %d\n", numParams);
	for(int i=0; i< numParams - 1; i++)
		{
			args[i] = strtok(args[i], "\n");
			//printf("%s\n", args[i]);
			//search child directories
			if(MF->attr==16 || !MF->attr)
			{
				found = searchDirectory(args[i], bootSector, MF);
				if (found == 0){
					return 0;
				}
			}
			else{
				if(i == numParams-2)
				{
					return 1;
				}
				else{
					return 0;
				}
			}
		}
	return 1;
}

int searchDirectory(char* filename, struct bootSector bootSector, struct fileData* MF)
{
	setvbuf (stdout, NULL, _IONBF, 0);
	int fin = 0;
	char* temp_string = malloc(strlen(filename));
	for(int i=0; i< strlen(filename); i++)
	{
		temp_string[i] = toupper(filename[i]);
	}

	filename = strcpy(filename, temp_string);
	free(temp_string);

	int firstSector = (bootSector.numFats * bootSector.sectorsPerFat) + 1;

	if(MF->filename != NULL){
		firstSector = MF->firstLogicalCluster;
	}
	while(fin == 0){
		char * sector = malloc(BYTES_PER_SECTOR*(sizeof(char)));
		read_sector(firstSector, sector);
		for(int j=0; j<16; j++)
		{
			MF->filename = malloc(8 * sizeof(char));
			MF->ext = malloc(3 * sizeof(char));
			for (int k = 0; k<8; k++)
			{
				MF -> filename[k] = sector[(j*32) + k];
				if(k == 0){
					if(sector[(j*32)] == (char)0x00)
					{
						fin = 1;
						break;
					}
				}
			}
			MF->filename = cleanFilename(MF->filename);
			for (int k = 0; k<3; k++)
			{
				MF->ext[k] = sector[(j*32)+ 8 + k];
			}

			MF->attr = sector[(j*32) + 11];
			MF->firstLogicalCluster = getBytes(sector, (j*32) + 26);
			MF->fileSize = getBigBytes(sector, (j*32) + 28);
			//printf("%s,%s, %d, %d, %d\n", MF->filename, MF->ext, MF-> attr, MF->firstLogicalCluster, MF->fileSize);
			if (strcmp(MF->filename, filename) == 0)
			{
				return 1;

			}
			free(MF->filename);
			free(MF->ext);
		}
		firstSector +=1;
	}
	return 0;
}

char * cleanFilename(char* mstr){
	//strips the random characters

	char ** args;
	int numParams = 1;
	char* newString;

	args = malloc(numParams * sizeof(char*));
	args[0] = strtok(mstr, " ");
	while (args[numParams - 1] != NULL) {
		//find number of parameters
		numParams += 1;
		args = realloc(args, numParams * sizeof(char*));
		args[numParams - 1] = strtok(NULL, " ");
	}
	numParams -= 1;
	if (numParams == 1){
		int val1 = strcmp(args[0], "Ae");
		int val2 = strcmp(args[0], "As");
		if (val1==0){
			newString = ".";
		}
		else if(val2==0){
			newString = "..";
		}
		else{
			newString = mstr;
		}
		//entry string was fine
	}
	else{
		int size = 0;
		for ( int i = 0; i<(numParams - 1); i++){
			size += strlen(args[i]);
		}
		newString = malloc(size * sizeof(char));
		for (int j = 0; j<(numParams - 1); j++){
			strcpy(newString, args[j]);
		}
	}
	mstr = malloc(strlen(newString));
	strcpy(mstr, newString);
	return mstr;
}
