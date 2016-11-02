/*
 * shell.h
 *
 *      Author: alextaxiera
 */

#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include "fatSupport.h"
#include "bootSector.h"
#include "fileData.h"

int shell();

char* readInput();
char** parseInput(char*);

int getBytes(char* boot, int val);
int getBigBytes(char* boot, int val);

//in pbs.c
struct bootSector readBootSector();
void pbs(struct bootSector bootSector);

//in pfe.c
void pfe(int min, int max, struct bootSector bootSector);
int checkRange(int min, int max);
char* readFatTable(struct bootSector bootSector, int x);

//in cd.c
char* cd(struct bootSector bootSector, char** fields, int num_params, char* currentDirectory);
char* getAbsolutePath(char* cwd, char* param);
int findTarget(char* abs_path, struct bootSector bootSector, struct fileData* MF);
int searchDirectory(char* filename, struct bootSector bootSector, struct fileData* MF);
char* cleanFilename(char* string);

#endif /* SHELL_H_ */
