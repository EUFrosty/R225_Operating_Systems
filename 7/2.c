#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
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


bool shouldTerminate = false;
void handleSIG(int signum){
	switch(signum){
		case SIGINT:
			printf("Stigao je sigint\n");
			break;
		case SIGTERM:
			printf("Stigao je sigterm\n");
			shouldTerminate = true;
			break;
		default:
			printf("Stiglo je nesto drugo\n");
	}
}

int main(int argc, char **argv){
	checkError(argc >= 2, "argc");
	printf("pid : %d\n", getpid());
	checkError(signal(SIGINT, handleSIG) != SIG_ERR, "signal");
	checkError(signal(SIGTERM, handleSIG) != SIG_ERR, "signal2");
	
	do{

		pause();

	}while(!shouldTerminate);

	return 0;
}