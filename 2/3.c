#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
bool myFopen(const char *pathname, const char *mode, int* fd){
    int flags = 0;
    int creation_mode = 0755;
    
    switch(mode[0]){
        case 'r':
            flags |= (mode[1] == '+') ? O_RDWR : O_RDONLY;
            break;
        case 'w':
            flags |= (mode[1] == '+') ? O_RDWR : O_WRONLY;
            flags |= O_CREAT;
            flags |= O_TRUNC;
            break;
        case 'a':
            flags |= (mode[1] == '+') ? O_RDWR : O_WRONLY;
            flags |= O_CREAT;
            flags |= O_APPEND;
            break;
    }
    
    *fd = open(pathname, flags, creation_mode);
    if(*fd < 0)
        return false;
    else
        return true;
}
    
int main(int argc, char** argv){
    int fd;
    checkError(argc == 3, "argc");
    
    checkError(myFopen(argv[2], argv[1], &fd), "myfopen");
    
    close(fd);
    return 0;
    
}
