#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	pid_t child_one, child_two;
	/* forking a child process */
	child_one = fork();
	if(child_one < 0) {
		fprintf(stderr, "First Child Failed to Fork\n");
		exit(EXIT_FAILURE);
	}
	else if(child_one == 0) {
		printf("I am child one, my pid is: %i\n", getpid());
	}
	else {
		/* forking second child */
		child_two = fork();
		if(child_two < 0) {
			fprintf(stderr, "Second Child Failed to Fork\n");
			exit(EXIT_FAILURE);
		}
		else if(child_two == 0) {
			printf("I am child two, my pid is: %i\n", getpid());
		}
		else {
			/* wait for both childs to finish*/
			/* parent code */
			wait(NULL);
		}
	}

}