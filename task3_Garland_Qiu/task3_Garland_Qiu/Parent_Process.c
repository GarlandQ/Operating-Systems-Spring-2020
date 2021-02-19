#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	int child_1, child_2;
	char *const argv[] = {"Process_P1", "Process_P2", NULL};

	child_1 = fork();
	if(child_1 < 0) {
		fprintf(stderr, "Child Process_P1 Failed\n");
		exit(EXIT_FAILURE);
	}
	else if(child_1 == 0) {
		execv(argv[0], argv);
	}
	else {
		child_2 = fork();
		if(child_2 < 0) {
			fprintf(stderr, "Child Process_P2 Failed\n");
			exit(EXIT_FAILURE);
		}
		else if(child_2 == 0) {
			execv(argv[1], argv);
		}
		else {
			wait(NULL);
		}
	}
}