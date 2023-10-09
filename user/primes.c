#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 35

void childsJobs(int fd[2], int check){
    //child process read from pipe
    if(check > MAX){
        exit(0);
    }

    int buf;
    // if the pipe is empty, exit
    if(!read(fd[0], &buf, sizeof(int))){
        exit(0);
    }
    printf("prime %d\n", buf);

    //create antoher pipe
    int fd2[2];
    pipe(fd2);
    int tmp;

    // write everything to the pipe before the fork happens
    while(read(fd[0], &tmp, sizeof(int))){
        //printf("in parent process, tmp is %d\n", tmp);
        if(tmp % check != 0){
            // printf("the process %d write %d to pipe\n", getpid(), tmp);
            write(fd2[1], &tmp, sizeof(int));
        }
    }

    //create a child process
    int pid = fork();
    if(pid != 0){
        //parent process
        close(fd2[0]);
        close(fd2[1]);
        wait(0);
    }else{
        //child process
        close(fd2[1]);
        childsJobs(fd2, check + 1);
        close(fd2[0]);

        // printf("debug child2 in %d, the buf is %d\n", getpid(), buf);
    }
    
}

int main(void){

    //create a pipe
    int fd[2];
    pipe(fd);

    //write to pipe before the fork happens
    for(int i = 2; i <= MAX; i++){
        write(fd[1], &i, sizeof(int));
    }

    //create a child process
    // printf("debug\n");
    int pid = fork();
    
    if(pid != 0){
        //parent process
        close(fd[0]);
        //write to pipe
        
        close(fd[1]);
        wait(0);
        // printf("debug parent in %d\n", getpid());
    }else{
        //child process
        close(fd[1]);
        childsJobs(fd, 2);
        close(fd[0]);
        // printf("debug child in %d\n", getpid());
    }

    exit(0);

}