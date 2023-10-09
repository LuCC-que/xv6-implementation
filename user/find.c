#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;

//   // Return blank-padded name.
//     if(strlen(p) >= DIRSIZ)
//         return p;
    memmove(buf, p, strlen(p) + 1);
    // printf("check what is buf before padding %s\n", buf);
    // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    // printf("check what is buf after padding %s\n", buf);
    return buf;
}



void search_for_file(char *path, char *target_file) {
    int fd;
    struct dirent de;
    struct stat st;

    

    char buf[512], *p;

    // printf("check what is path %s\n", path);

    if ((fd = open(path, 0)) < 0) {
        printf("find: failed to open %s\n", path);
        return;
    }
    
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(2);
    }

    switch (st.type)
    {
    case T_FILE:
        // printf("check 2 what is fmtname %s\n", fmtname(path));
        // printf("check what is fmtname %s\n", fmtname(path));
        // printf("check what is target_file %s\n", target_file);
        // printf("check what type is %d\n", st.type);
        // printf("check length of fmtname %d\n", strlen(fmtname(path)));
        // printf("check length of target_file %d\n", strlen(target_file));
        // printf("difference? %d\n", strcmp(fmtname(path), target_file));
        // check if it is the file we want
        
        if(strcmp(fmtname(path), target_file) == 0){
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("find: path too long\n");
            exit(2);
        }

        strcpy(buf, path);
        p = buf + strlen(buf); //p points to the end of buf
        *p++ = '/';

        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name, ".") == 0 
                    || strcmp(de.name, "..") == 0)
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; //end of buf is 0

            //recursively call search_for_file for every file in the directory
            // int pid = fork();
            // if(pid == 0){
            //     search_for_file(buf, target_file);
            //     exit(0);
            // }
            // printf("check what is buf %s\n", buf);
            
            search_for_file(buf, target_file);

        }
        break;
    
    }
    close(fd);

}


int main(int argc, char *argv[]){

    //the argv[1] is the directory
    //the argv[2] is the file name

    if(argc != 3){
        printf("usage: find <directory> <file name>\n");
        exit(0);
    }
    
    search_for_file(argv[1], argv[2]);

    free(argv[1]);
    free(argv[2]);
    free(argv);
    exit(0);

}
