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

#define MAX_SIZE 100

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
#define PARENT (child > 0)

int main(int argc, char **argv){
	checkError(argc != 0, "argc");
	int pipeFds[2];

	//0 - read
	//1 - write
	checkError(pipe(pipeFds)!=-1, "pipe");

	pid_t child = fork();
	checkError(child != -1, "fork");

	if(PARENT){
		close(pipeFds[READ_END]);

		FILE *fileStream = fdopen(pipeFds[WRITE_END], "w");
		checkError(fileStream != NULL, "fdopn1");

		char* message = "Hello my child!";
		int num = 12345;

		fprintf(fileStream, "Poruka: %s, broj: %d\n", message, num);

		fclose(fileStream);
		close(pipeFds[WRITE_END]);
			
	}else{
	close(pipeFds[WRITE_END]);

	FILE *fileStream = fdopen(pipeFds[READ_END], "r");
	checkError(fileStream != NULL, "fdopen2");

	char buffer[MAX_SIZE];
	fgets(buffer, MAX_SIZE, fileStream);
	printf("%s", buffer);
	
	fclose(fileStream);
	close(pipeFds[READ_END]);
	return 0;
	}
		


	checkError(wait(NULL) != -1, "wait");
	
	
	return 0;
}
