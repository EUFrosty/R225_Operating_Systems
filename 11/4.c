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
#include <stdatomic.h>

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
	char* putanja;
}ThreadArg;

atomic_int globalSum;


void *threadFunction(void *arg){
	char* filePath = arg;

	FILE *fd = fopen(filePath, "r");
	checkError(fd != NULL, "fopen");


	int broj = 0;
	int localSum = 0;
	while(fscanf(fd, "%d", &broj) != EOF){
		localSum += broj;
	}
	fclose(fd);

	
	atomic_fetch_add(&globalSum, localSum);
	

	return NULL;
}

int main(int argc, char **argv){
	checkError(argc >= 2, "argc");

	int numOfThreads = argc - 1;

	pthread_t *tids = malloc(numOfThreads * sizeof(pthread_t));
	checkError(tids != NULL, "malloc");

	atomic_init(&globalSum, 0);

	for(int i = 0; i < numOfThreads; i++){
		osPthreadCheck(pthread_create(&tids[i], NULL, threadFunction, argv[i+1]), "create");
	}

	
	for(int i = 0; i < numOfThreads; i++){
		osPthreadCheck(pthread_join(tids[i], NULL), "join");
	}
	
	printf("%d\n", atomic_load(&globalSum));


	free(tids);

	return 0;
}