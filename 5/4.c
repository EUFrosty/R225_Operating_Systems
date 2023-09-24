#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <string.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)

int main(int argc, char **argv){
    checkError(argc == 1, "argc");
    
    time_t now = time(NULL);
    
    char *nowStr = ctime(&now);
    checkError(nowStr != NULL, "ctime");
    
    struct tm *brokenTime = localtime(&now);
    checkError(brokenTime != NULL, "localtime");
    
    printf("%d.%d.%d.\n", brokenTime->tm_mday, brokenTime->tm_mon+1, brokenTime->tm_year+1900);
    
    return 0;
}
