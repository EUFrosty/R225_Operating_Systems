#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ftw.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define checkError(err, usrMsg)\
	do{\
		if(!(err)){\
			perror(usrMsg);\
			fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

#define READ_END (0)
#define WRITE_END (1)
int main(int argc, char **argv){

	pid_t child = fork();

	if(child == 0){
		execlp("gcc", "gcc", argv[1], NULL);
	}
	int status;
	waitpid(child, &status, 0);

	if(WIFEXITED(status)){
		int exitCode = WEXITSTATUS(status);

		if(exitCode != EXIT_SUCCESS){
			exit(exitCode);
		}

		struct stat fileinfo;
		stat("a.out", &fileinfo);
		printf("File size: %ld\n", fileinfo.st_size);
	}

	
	return 0;
}
