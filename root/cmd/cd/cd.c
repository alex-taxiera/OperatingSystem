#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <disk.h>
#include <path.h>

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Usage: cd [DIR]\n");
	    return 0;
	}

	if ( ! set_cwd(argv[1])) {
		printf("Invalid path\n");
		return 1;
	}

    return 0;
}
