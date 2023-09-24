//#define _XOPEN_SOURCE 700
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
#include <pthread.h>
#include <sys/wait.h>

#define checkError(err, usrMsg)\
	do{\
		if(!(err)){\
			perror(usrMsg);\
			fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

#define osPthreadCheck(pthreadErr, usrMsg)\
	do{\
		int _pthreadErr = pthreadErr;\
		if(_pthreadErr>0){\
			errno = _pthreadErr;\
			checkError(false, usrMsg);\
		}\
	}while(0)

typedef struct{
	int assignedNumber;
}ThreadArg;

void *threadFunction(void *arg){

	sleep(2);

	printf("Hello from detached thread\n");

	return NULL;
}

int main(int argc, char **argv){
	checkError(argc == 2, "argc");

	pthread_t backgroundThread;

	osPthreadCheck(pthread_create(&backgroundThread, NULL, threadFunction, NULL), "create");

	//osPthreadCheck(pthread_detach(backgroundThread), "create");

	osPthreadCheck(pthread_join(backgroundThread, NULL), "create");

	sleep(5);

	return 0;
}