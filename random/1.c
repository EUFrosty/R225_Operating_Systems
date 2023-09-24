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

	int par2cld[2];
	int cld2par[2];
	checkError(pipe(par2cld) != -1, "pipe1");
	checkError(pipe(cld2par) != -1, "pipe2");

	pid_t child = fork();

	if(child){
		close(par2cld[READ_END]);
		close(cld2par[WRITE_END]);


		char *line = NULL;
		int lineLen = 0;

		while(1){
			getline(&line, &lineLen, stdin);

			if(strcmp(line, "quit\n") == 0){
				kill(child, SIGKILL);
				printf("Bye\n");
				break;
			}
		

		write(par2cld[WRITE_END], line, strlen(line));

		int exitStatus = 0;
		char retVal[32];
		int readBytes = 0;

		readBytes = read(cld2par[READ_END], retVal, 32);

		retVal[readBytes] = '\0';

		sscanf(retVal, "%d", &exitStatus);
		if(WIFEXITED(exitStatus)){
			if(WEXITSTATUS(exitStatus)==EXIT_SUCCESS)
				printf("Success\n");
			else
				printf("Failure\n");
		}else{
			printf("Failure\n");
		}
		}


	}else{
		close(par2cld[WRITE_END]);
		close(cld2par[READ_END]);

		char command[20];

		char response[32];

		freopen("/dev/null","w",stdout);
		freopen("/dev/null","w",stderr);

		while(1){
			memset(command, 0, 20);

			read(par2cld[READ_END], command, 20);

			int retVal = system(command);

			sprintf(response, "%d", retVal);

			write(cld2par[WRITE_END], response, strlen(response));
		}


		close(par2cld[READ_END]);
		close(cld2par[WRITE_END]);

	}

	wait(NULL);

	
	return 0;
}