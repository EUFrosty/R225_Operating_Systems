#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
int main(int argc, char **argv){
    checkError(argc == 3, "argc");
    
    int fd = open(argv[1], O_WRONLY);
    checkError(fd != -1, "open");
    
    int fileFlags = fcntl(fd, F_GETFL);
    
    fileFlags = fileFlags | O_APPEND;
    
    checkError(fcntl(fd,F_SETFL, fileFlags) != -1, "fcntl");
    write(fd, "999", 3);
    return 0;
}
