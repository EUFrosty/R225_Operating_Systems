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
	checkError(argc >= 2, "argc");

	FILE *fileStream = fopen(argv[1], "r+");
	checkError(fileStream != NULL, "fopen");

	int fd = fileno(fileStream);
	checkError(fd != -1, "fileno");

	struct stat fileInf;
	checkError((fstat(fd, &fileInf)) != -1, "fstat");

	printf("Filesize: %ld\n", fileInf.st_size);

	fclose(fileStream);
	close(fd)
	
	return 0;
}