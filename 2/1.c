#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
int main(int argc, char **argv){
    checkError(argc == 2, "usage: ./a.out ime_fajla");
    
    int mode = 0755; //ovo se tretira kao broj zapisan u osnovi 8, 0x755 bi bio u osnovi 16
    
    checkError(mkdir(argv[1], mode) != -1, "mkdir");
    
    return 0;
}
