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

int main(int argc, char **argv){

	FILE *fd = fopen(argv[1], "r");
	float sum = 0.0;
	float number = 0.0;

	while(fscanf(fd, "%f", &number) == 1){
		sum += number;
	}

	checkError(feof(fd), "feof");

	printf("Sum: %f\n", sum);
	
	return 0;
}