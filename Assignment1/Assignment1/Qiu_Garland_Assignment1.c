#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	/* the array fd will let us read and write over a pipe */
	/* a[0] is for reading the pipe and a[1] is for writing*/
	int fd[2];
	/* pipe to process the communication */
	int pipe_p = pipe(fd);
	if(pipe_p < 0) {
		/* error occured */
		fprintf(stderr, "Pipe Failed\n");
		exit(EXIT_FAILURE);
	}

	pid_t child_a, child_b;
	/* forking a child process */
	child_a = fork();
	if(child_a < 0) {
		fprintf(stderr, "First Child Fork Failed\n");
		exit(EXIT_FAILURE);
	}
	else if(child_a == 0) {
		/* child process executes */
		/* close normal stdout*/
		close(1);
		/* duplicate a[1] to stdout*/
		dup(fd[1]);
		/* close the reading part of the pipe since we don't need it afterwards*/
		int close_read = close(fd[0]);
		/* if closing fails, return an error*/
		if(close_read < 0) {
			fprintf(stderr, "Closing Pipe Failed\n");
			exit(EXIT_FAILURE);
		}
		else {
			execlp("ls", "ls", "-F", NULL);
		}
	}
	
	else {
		/* second child will now start*/
		child_b = fork();
		if(child_b < 0) {
			fprintf(stderr, "Second Child Fork Failed\n");
			exit(EXIT_FAILURE);
		}
		else if(child_b == 0) {
			/* second child process executes*/
			/* close normal stdin*/
			close(0);
			/* duplicate a[0] to stdin*/
			dup(fd[0]);
			/* close the writing part of the pipe since we don't need it afterwards*/
			int close_write = close(fd[1]);
			/* if closing fails, return an error */
			if(close_write < 0) {
				fprintf(stderr, "Closing Pipe Failed\n");
				exit(EXIT_FAILURE);
			}
			else {
				execlp("nl", "nl", NULL);
			}
		}
		else { 
			/* parent waits for both child processes*/
			wait(NULL);
		}
	}
	return 0;
}