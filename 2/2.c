#include <stdio.h>
#include <stdlib.h>
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
    
    if(argv[1][0] == 'f'){
        checkError(-1 != unlink(argv[2]), "unlink");
    }else if(argv[1][0] == 'd'){
        checkError(rmdir(argv[2]) != -1, "rmdir");
    }
    
    return 0;
}
