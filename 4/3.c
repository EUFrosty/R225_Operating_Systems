#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)

void sizeOfDir(const char *putanja, unsigned *dirSize){
    
    struct stat fileinfo;
    checkError(lstat(putanja, &fileinfo) != -1, "stat"); 
    
    if(!S_ISDIR(fileinfo.st_mode)){
        *dirSize += fileinfo.st_size;
        return;
    }else{
        *dirSize += fileinfo.st_size;
    }
    
    DIR *dir = opendir(putanja);
    checkError(dir != NULL, "opendir");
    
    struct dirent *dirEntry = NULL;
    
    errno = 0;
    while((dirEntry = readdir(dir)) != NULL){
        char *path = malloc(strlen(putanja) + 1 + strlen(dirEntry->d_name) + 1);
        checkError(path != NULL, "malloc");
        
        strcpy(path, putanja);
        strcat(path, "/");
        strcat(path, dirEntry->d_name);
        
        if(strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0){
            checkError(stat(path, &fileinfo) != -1, "stat");
            
            *dirSize += fileinfo.st_size;
            free(path);
            errno = 0;
            continue;
        }
        sizeOfDir(path, dirSize);
        free(path);
        errno = 0;
    }
    checkError(errno != EBADF, "readdir");
    
    checkError(closedir(dir) != -1, "closedir");
    return;
}    

    
int main(int argc, char **argv){
    checkError(argc == 2, "argc");
    
    struct stat fileinfo;
    checkError(stat(argv[1], &fileinfo) != -1, "stat");
    
    if(!S_ISDIR(fileinfo.st_mode))
        checkError(false, "nije dir");
    
    unsigned dirSize = 0;
    sizeOfDir(argv[1], &dirSize);
    
    printf("%u\n", dirSize);
    
    return 0;
}
