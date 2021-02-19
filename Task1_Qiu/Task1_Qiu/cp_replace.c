#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("\n Not enough given arguments \n");
		return 1;
	}
	char *filepath = argv[1];
	char buffer[99];
	int count;
	int i;
	//Check if the file exists
	int exist = access(filepath, F_OK);
	if(exist == 0) {
		int readfile = open(filepath, O_RDONLY);
		if(readfile == -1)
			printf("\n %s is not readable \n", filepath);
		else {
			int destfile = creat(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) | open(argv[2], O_RDWR);
			if(destfile == -1) {
				printf("\n creat() failed with error [%s] \n", strerror(errno));
			}
			else {
				destfile = open(argv[2], O_RDWR);
				if(destfile != -1) {
					count = read(readfile, buffer, 100);
					printf("\n %i \n",count);
					while((count = read(readfile, buffer, 100)) > 0) {
						i = 0;
						while(i < count) {
							if(buffer[i] == '1')
								buffer[i] = 'A';
							i++;
						}
						if(write(destfile, buffer, count) == -1)
							printf("\n Write function failed. \n");
						int xyz_write = write(destfile, "XYZ", strlen("XYZ"));
						if(xyz_write == -1)
							printf("\n XYZ write failed. \n");
					}
					printf("\n %i \n",count);
					printf("\n%s copied over to %s \n", filepath, argv[2]);
					int close_destfile = close(destfile);
					if(close_destfile == 0) {
						printf("\n %s successfully closed\n", argv[2]);
					}
				}
				else {
					printf("\n cannot open the new destination file \n");
				}
			}
		}
		close(readfile);
	}
	else {
		printf("\n %s does not exist \n", filepath);
	}
	return 0;
}