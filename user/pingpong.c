#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define COMMONBEHAVIOR(file, buf) \
        close(file); \
        /* print the message */ \
        printf("%d: received %s\n", getpid(), buf); \
        /* free buf */ \
        free(buf); \
        /* child close the write side and quit */ \
        exit(0);


int main(void){

    //create two file descriptors
    //one for write another for read

    int fd[2];

    //create a pipe
    pipe(fd);

    //fork a child process

    int pid = fork();

    //if pid == 0, it is child process
    if(pid == 0){

        //child process wait for parent process to write
        char buf[4];
        read(fd[0], buf, 4);
        
        //child process write to pipe
        write(fd[1], "pong", 4);

        COMMONBEHAVIOR(fd[1], buf);

    }
   
            
    //parent process write to pipe
    write(fd[1], "ping", 4);

    //parent process wait for child process to read
    wait(0);

    //parent process read from pipe
    char buf[4];
    read(fd[0], buf, 4);

    COMMONBEHAVIOR(fd[1], buf);

    


}