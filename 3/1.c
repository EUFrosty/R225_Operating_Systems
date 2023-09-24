#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>

#define MAX 1024

#define checkError(err, usrMsg)\
    do{\
        if(!(err)){\
            perror(usrMsg);\
            fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
void printUserInfo(const char *username, FILE *outStream){
    struct passwd *userInfo = NULL;
    
    userInfo = getpwnam(username);
    checkError(userInfo != NULL, "getpwnam");
    
    fprintf(outStream, "Username: %s\n", userInfo->pw_name);
    fprintf(outStream, "Password: %s\n", userInfo->pw_passwd);
    fprintf(outStream, "UserID: %d\n", userInfo->pw_uid);
    fprintf(outStream, "GroupID: %d\n", userInfo->pw_gid);
    fprintf(outStream, "Info: %s\n", userInfo->pw_gecos);
    fprintf(outStream, "Directory: %s\n", userInfo->pw_dir);
    fprintf(outStream, "Shell: %s\n", userInfo->pw_shell);
}

void printAllUsers(FILE *outStream){
    struct passwd *userInfo = NULL;
    
    setpwent();
    
    while((userInfo = getpwent()) != NULL){
        fprintf(outStream, "Username: %s\n", userInfo->pw_name);
        fprintf(outStream, "Password: %s\n", userInfo->pw_passwd);
        fprintf(outStream, "UserID: %d\n", userInfo->pw_uid);
        fprintf(outStream, "GroupID: %d\n", userInfo->pw_gid);
        fprintf(outStream, "Info: %s\n", userInfo->pw_gecos);
        fprintf(outStream, "Directory: %s\n", userInfo->pw_dir);
        fprintf(outStream, "Shell: %s\n", userInfo->pw_shell);
        
    }
    
    endpwent();
    return;
}

void printAllGroups(FILE *outStream){
    struct group *groupInfo = NULL;
    
    setgrent();
    
    while((groupInfo = getgrent()) != NULL){
        fprintf(outStream, "Group name: %s\n", groupInfo->gr_name);
        fprintf(outStream, "Password: %s\n", groupInfo->gr_passwd);
        fprintf(outStream, "GroupID: %d\n", groupInfo->gr_gid);
        
        for(int i = 0; groupInfo->gr_mem[i] != NULL; i++)
            fprintf(outStream, "%s \t", groupInfo->gr_mem[i]);
        fprintf(outStream, "\n");
        
    }
    
    endgrent();
    return;
}

char *getUsername(uid_t uid){
    struct passwd *userInfo = getpwuid(uid);
    checkError(userInfo != NULL, "getpwuid");
    
    char *tmpName = malloc(MAX * sizeof(char));
    checkError(tmpName != NULL, "malloc");
    
    strcpy(tmpName, userInfo->pw_name);
    
    return tmpName;
}

void getFileInfo(const char *filename, char **result){
    *result = malloc(MAX * sizeof(char));
    checkError(*result != NULL, "malloc");
    
    char *resultStr = *result;
    
    struct stat fileinfo;
    checkError(stat(filename, &fileinfo)!= -1, "stat");
    
    //FILE TYPE
    if(S_ISREG(fileinfo.st_mode))
        resultStr[0] = '-';
    else if(S_ISDIR(fileinfo.st_mode))
        resultStr[0] = 'd';
    
    //PERMISSIONS
    if(fileinfo.st_mode & S_IRUSR)
        resultStr[1] = 'r';
    else
        resultStr[1] = '-';
    
    if(fileinfo.st_mode & S_IWUSR)
        resultStr[2] = 'w';
    else
        resultStr[2] = '-';
    
    if(fileinfo.st_mode & S_IXUSR)
        resultStr[3] = 'x';
    else
        resultStr[3] = '-';
    
    if(fileinfo.st_mode & S_IRGRP)
        resultStr[4] = 'r';
    else
        resultStr[4] = '-';
    
    if(fileinfo.st_mode & S_IWGRP)
        resultStr[5] = 'w';
    else
        resultStr[5] = '-';
    
    if(fileinfo.st_mode & S_IXGRP)
        resultStr[6] = 'x';
    else
        resultStr[6] = '-';
    
    if(fileinfo.st_mode & S_IROTH)
        resultStr[7] = 'r';
    else
        resultStr[7] = '-';
    
    if(fileinfo.st_mode & S_IWOTH)
        resultStr[8] = 'w';
    else
        resultStr[8] = '-';
    
    if(fileinfo.st_mode & S_IXOTH)
        resultStr[9] = 'x';
    else
        resultStr[9] = '-';

    //LINKS
    int offset = 10;
    offset += sprintf(resultStr + offset, " %ld ", fileinfo.st_nlink);
    
    //USERNAME
    char *username = getUsername(fileinfo.st_uid);
    offset += sprintf(resultStr + offset, "%s ", username);
    
    //USERNAME
    offset += sprintf(resultStr + offset, "%s ", username);
    
    //SIZE
    offset += sprintf(resultStr + offset, "%ld ", fileinfo.st_size);
    
    //TIME
    time_t mtimeSec = fileinfo.st_mtime;
    char *timeStr = ctime(&mtimeSec);
    offset += sprintf(resultStr + offset, "%s ", timeStr);
    
}
    
int main(int argc, char **argv){
    
    checkError(argc == 2, "argc");
    char *result = NULL;
    
    getFileInfo(argv[1], &result);
    printf("%s\n", result);
    
    free(result);
    
    return 0;
}
