#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
    
int main(int argc, char **argv){
    checkError(argc == 2, "argc");
    
    mkdir("8", 0777);
    
    return 0;
}
