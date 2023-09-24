
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

	int fd = open(argv[1], O_WRONLY);
	checkError(fd != -1, "open");

	char localbuf[20];

	do{
		write(fd, "abc", 3);

		printf("continue sending? (yes/no)\n");
		scanf("%s", localbuf);
	}while(strcmp(localbuf, "yes") == 0);

	close(fd);
	
	return 0;
}