#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int filedes[2], ret;
    int pid;
    char buffer[256];
    ret = pipe(filedes);
    if (ret != 0) {
        fprintf(0, "failed to create pipe %d", ret);
        exit(1);
    }
    pid = fork();
    // fprintf(0, "------%d\n", pid);
    if (pid == 0) {
        read(filedes[0], buffer, 1);
        pid = getpid();
        fprintf(1, "%d: received ping\n", pid);
        write(filedes[1], "B", 1);
    }else {
        write(filedes[1], "H", 1);
        sleep(1);
        read(filedes[0], buffer, 1);
        pid = getpid();
        fprintf(1, "%d: received pong\n", pid);
    }  
    exit(0);
}
