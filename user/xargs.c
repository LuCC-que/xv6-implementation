#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){

    //collect the arguments
    char *args[MAXARG];
    for(int i = 1; i < argc; i++){
        args[i-1] = argv[i];
    }
    args[argc-1] = 0;

    

    //collect the arguments from stdin into args
    char buf[512];
    int i = 0;
    int section = 0;
    // int count = argc;
    while(read(0, &buf[i], 1) > 0){
        //start reading from stdin
        
        if(buf[i] == '\n'){
            buf[i] = 0;
            args[argc - 1] = &buf[section];
            section = i+1;
            argc += 1;
            // printf("collected arguments from stdin: %s\n", args[argc-1]);
        }
        i++;
    }

    //null terminator
    args[argc] = '\0';

    //print the total arguments
    // for(int i = 0; i < argc; i++){
    //     printf("args[%d]: %s\n", i, args[i]);
    // }
    // printf("\n");

    //fork a child process
    int pid = fork();
    if(pid < 0){
        exit(1);
    }else if(pid == 0){
        //child process
        exec(argv[1], args);
        exit(1);
    }else{
        //parent process
        wait(0);
    }

    exit(0);

}