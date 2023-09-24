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
#include <sys/wait.h>

#define MAX 1024

#define checkError(err, usrMsg)\
	do{\
		if(!(err)){\
			perror(usrMsg);\
			fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

pid_t uradi(char *rec1, char *rec2){
	int pipeFds[2];

	checkError(pipe(pipeFds) != -1, "pipe");

	pid_t child = fork();

	if(child == -1)
		return -1;
	if(child == 0){
		close(pipeFds[1]);
		checkError(execlp(rec1, rec1, rec2, NULL), "exec nije uspeo");
		return -1;
	}else{
		close(pipeFds[0]);
		wait(NULL);
		return child;
	}
}



int main(int argc, char **argv){
	checkError(argc == 2, "argc");

	char rec1[100];
	char rec2[100];
	int c;

	FILE *fd = fopen(argv[1], "r");
	checkError(fd != NULL, "fopen");

	while(c != EOF){
		int i = 0;
		int j = 0;

		while((c = fgetc(fd)) != ' ' && c != EOF)
			rec1[i++] = c;
		rec1[i] = '\0';

		while((c = fgetc(fd)) != '\n' && c != EOF)
			rec2[j++] = c;
		rec2[j] = '\0';
		uradi(rec1, rec2);
	}



	fclose(fd);

	return 0;
}