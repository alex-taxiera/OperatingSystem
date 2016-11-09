/*
 * shell.c
 *
 *      Author: alextaxiera
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "shell.h"

int shell(int argc, char *argv[])
{
	if (FILE_SYSTEM_ID == NULL) {
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	char* currentDirectory;
	currentDirectory = malloc(sizeof(char));
	currentDirectory = "/";
	struct bootSector bootSector = readBootSector();
    int exitShell = 1;

    do
    {
    	char buffer[128];
        printf("➜ ");
		fgets(buffer, 128, stdin);
		char ** args;
		int numParams = 1;

        args = malloc(numParams * sizeof(char*));
        		args[0] = strtok(buffer, " ");
        		while (args[numParams - 1] != NULL) {
        			//find number of parameters
        			numParams += 1;
        			args = realloc(args, numParams * sizeof(char*));
        			args[numParams - 1] = strtok(NULL, " ");
        		}

        		for (int i = 0; i < numParams; i++) {
        			args[i] = strtok(args[i], "\n");
        		}
        		numParams -= 1;

          if (args[0] == NULL)
          {
            // An empty command was entered.
            return 1;
          }

          if (strcmp(args[0], "cd") == 0) {
        		if (numParams == 2 || numParams == 1) {
        			currentDirectory = cd(bootSector, args, numParams, currentDirectory);
        		} else {
        			printf("Command %s take one or no params.\n", args[0]);
        		}
        	} else if (strcmp(args[0], "ls") == 0) {
        		if (numParams == 2 || numParams == 1) {
        			pid_t pid = fork();
        			if (pid == 0) {
        				printf("Child process for %s.\n", args[0]);
        				printf("Child process killed\n");
        			}
        		} else {
        			printf("Command %s takes up to one additional parameter.\n",
        					args[0]);
        		}
        	} else if (strcmp(args[0], "pbs") == 0) {
        		if (numParams == 1) {
        			pid_t pid = fork();
        			if (pid == 0) {
        				pbs(bootSector);
        			}
        		} else {
        			printf("Command %s takes no parameters.\n", args[0]);
        		}
        	} else if (strcmp(args[0], "pfe") == 0) {
        		if (numParams == 3) {
        			pid_t pid = fork();
        			if (pid == 0) {
        				printf("Child process for %s.\n", args[0]);
        				pfe(atoi(args[1]), atoi(args[2]), bootSector);
        			}
        		} else {
        			printf("Command %s requires two additional parameters.\n",
        					args[0]);
        		}
        	} else if (strcmp(args[0], "pwd") == 0) {
        		if (numParams == 1) {
        			pid_t pid = fork();
        			if (pid == 0) {
        				printf("%s\n", currentDirectory);
        			}
        		} else {
        			printf("Command %s takes no parameters.\n", args[0]);
        		}
        	} else if (strcmp(args[0], "exit") == 0) {
        		exitShell = 0;
        	} else {
        		printf("Command %s does not exist.\n", args[0]);
        	}


    } while (exitShell == 1);

	free(bootSector.volumeId);
	free(bootSector.volumeLabel);
	free(bootSector.systemType);
    return EXIT_SUCCESS;
}

int getBytes(char* boot, int val) {
	int mostSignificantBits;
	int leastSignificantBits;
	int bytesPerSector;

	mostSignificantBits = (((int) boot[val + 1]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int) boot[val]) & 0x000000ff;
	bytesPerSector = mostSignificantBits | leastSignificantBits;

	return bytesPerSector;
}

int getBigBytes(char* boot, int val) {

	int mostSignificantBits;
	int leastSignificantBits;
	int almostSignificantBits;
	int atleastSignificantBits;

	almostSignificantBits = (((int) boot[val + 3]) << 24) & 0xff000000;
	atleastSignificantBits = ((int) boot[val +2] << 16) & 0x00ff0000;
	mostSignificantBits = (((int) boot[val + 1]) << 8) & 0x0000ff00;
	leastSignificantBits = ((int) boot[val]) & 0x000000ff;
	return almostSignificantBits | atleastSignificantBits | mostSignificantBits|leastSignificantBits;
}
