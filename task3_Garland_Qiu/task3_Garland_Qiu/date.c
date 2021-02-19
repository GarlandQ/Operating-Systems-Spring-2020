#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	pid_t child;
	child = fork();
	if(child < 0) {
		fprintf(stderr, "Child Process Failed\n");
		exit(EXIT_FAILURE);
	}
	else if(child == 0) {
		printf("Child pid: %i\n", getpid());
		execl("/bin/date", "date", 0, NULL);
	}
	else {
		wait(NULL);
	}
}