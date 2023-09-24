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

int main(int argc, char **argv){
	checkError(argc == 1, "argc");
	int var = 5;

	int pipeFds[2];

	//pipe[0] read end
	//pipe[1] write end
	checkError(pipe(pipeFds) != -1, "pipe");

	pid_t child = fork();
	checkError(child != -1, "fork");

	if(child == 0){
		close(pipeFds[1]);

		char line[MAX];
		int readBytes = 0;

		checkError((readBytes = read(pipeFds[0], line, MAX)) != -1, "read");

		line[readBytes] = '\0';

		printf("%s", line);
		close(pipeFds[0]);

		return 0;
	}
	close(pipeFds[0]);
	
	char *line = NULL;
	size_t size = 0;

	checkError(getline(&line, &size, stdin) != -1, "getline");

	checkError(write(pipeFds[1], line, strlen(line)) != -1, "write");

	checkError(wait(NULL) != -1, "wait");

	close(pipeFds[1]);
	free(line);

	return 0;
}