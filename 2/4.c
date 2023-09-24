#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFF_SIZE (4096)

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
    
bool myCat(int fd){
    int readBytes = 0;
    char *memBuf = malloc(BUFF_SIZE * sizeof(char));
    if(memBuf == NULL)
        return false;
    
    while ((readBytes = read(fd, memBuf, BUFF_SIZE)) > 0){
        if(write(STDOUT_FILENO, memBuf, readBytes) == -1)
            free(memBuf);
            return false;
    }
    free(memBuf);
    
    if(readBytes < 0)
        return false;
    else 
        return true;
}

bool myCp(const char *dest, const char *source){
    int readBytes = 0;
    char *memBuf = malloc(BUFF_SIZE * sizeof(char));
    if(memBuf == NULL)
        return false;
    
    int fdSrc, fdDest;
    if(myFopen(source, "r", &fdSrc) != true || myFopen(dest, "w", &fdDest) != true){
        close(fdSrc);
        free(memBuf);
        return false;
    }
    
    while ((readBytes = read(fdSrc, memBuf, BUFF_SIZE)) > 0){
        if(write(fdDest, memBuf, readBytes) == -1)
            free(memBuf);
            return false;
    }
    free(memBuf);
    close(fdSrc);
    close(fdDest);
    
    if(readBytes < 0)
        return false;
    else 
        return true;
    
}

int main(int argc, char **argv){
    int fd;
    checkError(argc == 2, "argc");
    
    checkError(myFopen(argv[1], "r", &fd), "myfopen");
    
    checkError(myCat(fd), "mycat");
    
    close(fd);
    
    return 0;
}
