
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

	errno = 0;
	int res = mkfifo(argv[1], 0600);

	if(res == -1){
		checkError(errno == EEXIST, "mkfifo");

		struct stat fileinfo;
		checkError(stat(argv[1], &fileinfo) != -1, "stat");
		checkError(S_ISFIFO(fileinfo.st_mode), "nije fifo");
	}

	int fd = open(argv[1], O_RDONLY);
	checkError(fd = -1, "open");
	
	printf("OVDE\n");

	int readBytes = 0;
	char localbuf[20];

	while((readBytes = read(fd, localbuf, 20)) > 0){
		printf("Recieved: %s\n", localbuf);
	}
	checkError(readBytes != -1, "read");
	close(fd);
	
	return 0;
}