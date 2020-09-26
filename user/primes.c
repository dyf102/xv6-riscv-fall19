#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// #include <stdio.h>
//         #include <unistd.h>
//        #include <sys/types.h>
//#include <stdlib.h>
#define stderr 0
#define stdout 1

void job(int read_fd);

int
main(int argc, char *argv[])
{
    int filedes[2], ret;
    int status;
    status = 0;
    // char buffer[4];
    ret = pipe(filedes);
    if (ret != 0) {
        fprintf(stderr, "failed to create pipe %d", ret);
        exit(1);
    }
    for(int i = 2; i <= 35; i ++) {
        write(filedes[1], &i, sizeof(i));
    }
    // sleep(1);
    close(filedes[1]);
    job(filedes[0]);
    wait(&status);
    exit(0);
}

void job(int read_fd) {
    int filedes[2];
    int ret, pid, num; 
    char buffer[4];
    //
    sleep(1);
    ret = read(read_fd, buffer, 4);
    if (ret == 0) {
        fprintf(stdout, "pipe closed\n");
        return;
    }

    pid = getpid();
    num = *(int*)buffer;
    // write(0, buffer, 4);
    fprintf(stdout, "%d\n", num);
    ret = pipe(filedes);
    if (ret == -1){
        fprintf(stderr, "failed to create pipe\n");
        exit(-1);
    }
    pid = fork();
    if(pid == -1) {
        fprintf(stderr, "failed to fork\n");
        close(filedes[0]);
        close(filedes[1]);
        exit(-1);
    }else if(pid == 0) {
        // int pid = getpid();
        close(filedes[0]);
        while((ret = read(read_fd, buffer, 4)) != 0) {
             if(*(int*)buffer %num != 0) {
                //fprintf(stdout, "%d\n", *(int*)buffer);
                write(filedes[1], buffer, 4);
             }
        }
    }else {
        
        close(filedes[1]);
        
        job(filedes[0]);
        //close(read_fd);
        //close(filedes[0]);
    }
}


