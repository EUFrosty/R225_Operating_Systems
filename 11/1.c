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
	ThreadArg *data = arg;

	printf("My TID is %lu, and my assigned number is %d\n", pthread_self(), data->assignedNumber);

	return NULL;
}

int main(int argc, char **argv){
	checkError(argc == 2, "argc");

	int noOfThreads = atoi(argv[1]);

	pthread_t *tids = malloc(noOfThreads * sizeof(pthread_t));
	checkError(tids != NULL, "malloc");

	ThreadArg *data = malloc(noOfThreads * sizeof(ThreadArg));
	checkError(data != NULL, "malloc");

	for(int i = 0; i < noOfThreads; i++){
		data[i].assignedNumber = i;

		osPthreadCheck(pthread_create(&tids[i], NULL, threadFunction, &data[i]), "create");
	}


	for(int i = 0; i < noOfThreads; i++){
		
		osPthreadCheck(pthread_join(tids[i], NULL), "create");
	}

	free(tids);
	free(data);

	return 0;
}