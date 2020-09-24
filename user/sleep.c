#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        write(0, "Please specify the sleep length\n", 32);
        exit(1);
    }
    int i = atoi(argv[1]);
    if (i < 0) {
        write(0, "Please give a valid sleep length\n", 32);
        exit(1);
    }
    
    int ret = sleep(i);
    fprintf(2, "debug: %d\n", ret);
    exit(0);
}
