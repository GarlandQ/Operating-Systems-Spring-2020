#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int fd1 = open("destination1.txt", O_RDWR | O_CREAT, 0777);
	int fd2 = open("destination2.txt", O_RDWR | O_CREAT, 0777);

	if(-1 == fd1)
	{
		printf("\n file failed to be created or opened\n");
	}
	else {
		printf("\n destination1.txt created succesfully\n");
	}

	if(-1 == fd2)
	{
		printf("\n file failed to be created or opened\n");
	}
	else {
		printf("\n destination2.txt created succesfully\n");
	}

	return 0;
}