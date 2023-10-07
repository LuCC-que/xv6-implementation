#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){

    if(argc > 2){
        fprintf(2, "Usage: sleep [number]\n");

        exit(1);
    }

    // try to convert the argument to an integer
    int sleep_time = atoi(argv[1]);

    // if the argument is not an integer, atoi returns 0
    if (sleep_time == 0){
        fprintf(2, "Usage: sleep [number], the number has to be \
                        a postive integer that greater than 0\n");
        exit(1);
    }

    // sleep for the given time
    sleep(sleep_time);

    // exit with 0
    exit(0);
}