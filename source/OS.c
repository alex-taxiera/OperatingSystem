#include <stdio.h>
#include "shell.h"
#include "fatSupport.h"

//pick floppy disk
int main(int argc, char * argv[]) {
	int file_choice = 0;
	if (argc == 2 || argc == 1) {
		int file_choice = 0;
		if (argc > 1) {
			file_choice = atoi(argv[1]);
		}
		if (file_choice > 3 || file_choice < 0) {
			file_choice = 0;
		}
		switch (file_choice) {
		case 0:
		case 1:
			FILE_SYSTEM_ID = fopen("floppy1", "r+");
			printf("floppy 1\n");
			break;
		case 2:
			FILE_SYSTEM_ID = fopen("floppy2", "r+");
			printf("floppy 2\n");
			break;
		case 3:
			FILE_SYSTEM_ID = fopen("floppy3", "r+");
			printf("floppy 3\n");
			break;
		default:
			printf("error in handler\n");
			break;
		}
		//once floppy is loaded run shell
		shell();
	} else if (argc > 2) {
		printf("Incorrect number of arguments\n");
		return 1;
	}
	return 0;
}
