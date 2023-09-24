#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)

int days = 0;
    
int filterByTime(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    time_t lastMod = sb->st_mtime;
    time_t now = time(NULL);
    
    if((now - lastMod)/(24*60*60) < days && ftwbuf->level == 2){
        printf("%s\n", fpath);
    }
    return 0;
}    
    

int main(int argc, char **argv){
    checkError(argc == 3, "argc");
    
    struct stat fileinfo;
    checkError(stat(argv[1], &fileinfo) != -1, "stat");
    if(!S_ISDIR(fileinfo.st_mode))
        checkError(false, "nije dir");
    
    days = atoi(argv[2]);
    
    checkError(nftw(argv[1], filterByTime, 50, FTW_CHDIR | FTW_PHYS) != -1, "nftw");
    
    return 0;
}
