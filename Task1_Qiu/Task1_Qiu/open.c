#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	// if(argc != 2)
	// {
	// 	printf("\n Needs two arguments \n");
	// 	return 1;
	// }
	errno = 0;
	fd = open(argv[1], O_RDONLY | O_CREAT);
	// fd = creat(argv[1], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) | open(argv[1], O_RD);
	if(-1 == fd)
	{
		printf("\n creat() failed with error [%s]\n",strerror(errno));
	}

	fd = open(argv[1], O_RDONLY);
	if(fd != -1) {
		printf("\n file opened on descriptor \n");
		int closed = close(fd);
		if(closed == 0)
			printf("\n file closed on descriptor \n");
	}
	else
		printf("\n file failed to open \n");

	return 0;
}