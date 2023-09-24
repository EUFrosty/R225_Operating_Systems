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

void *createMemoryBlock(const char *filePath, unsigned size){
	int fd = shm_open(filePath, O_RDWR | O_CREAT, 0600);
	checkError(fd != -1, "shm_open");

	checkError(ftruncate(fd, size) != -1, "ftruncate");

	void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, );
}

int main(int argc, char **argv){
	checkError(argc >= 2, "argc");
	

	
	return 0;
}