#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int exist;
	int keep_going = 1;
	int switch_destination = 1;
	char* buffer = malloc(100);
	char* source = "source.txt";
	char* destination_1 = "destination1.txt";
	char* destination_2 = "destination2.txt";

	exist = access(source, R_OK);
	if(exist != 0) {
		printf("\n source.txt cannot be accessed\n");
	}

	int open_source = open(source, O_RDONLY);
	exist = access(destination_1, W_OK);
	if(exist != 0) {
		printf("\n destination1.txt cannot be accessed\n");
	}

	int write_destination1 = open(destination_1, O_WRONLY);
	exist = access(destination_2, W_OK);
	if(exist != 0) {
		printf("\n destination2.txt cannot be accessed\n");
	}

	int write_destination2 = open(destination_2, O_WRONLY);
	while(keep_going == 1) {
		keep_going = 0;
		if(switch_destination == 1) {
			int read_source = read(open_source, &buffer[0], 100);
			write(write_destination1, buffer, 100);

			if(read_source == 100) {
				keep_going = 1;
				switch_destination = 0;
			}
		}

		else {
			int read_source = read(open_source, &buffer[0], 50);
			write(write_destination2, buffer, 50);

			if(read_source == 50) {
				keep_going = 1;
				switch_destination = 1;
			}
		}
	}


	close(open_source);
	close(write_destination1);
	close(write_destination2);

	return 0;
}