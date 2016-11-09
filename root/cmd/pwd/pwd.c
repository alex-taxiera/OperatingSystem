#include <stdlib.h>
#include <string.h>
#include <disk.h>
#include <path.h>

int main(int argc, char** argv) {

    char * cwd = get_cwd();
    printf("%s\n", cwd);
    free(cwd);

    return 0;
}
