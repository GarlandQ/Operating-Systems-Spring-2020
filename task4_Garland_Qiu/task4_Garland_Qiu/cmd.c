#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int child, i;
	char command[100];
	char *token;
	char *array[50] = {NULL};

	printf("Enter cmd: ");
	scanf("%[^\n]s", command);

	while(strcmp(command, "quit") != 0) {
		token = strtok(command, " ");
		i = 0;
		while(token != NULL) {
			array[i] = token;
			token = strtok(NULL, " ");
			i++;
		}

		child = fork();
		if(child < 0) {
			fprintf(stderr, "First Child Failed to Fork\n");
			exit(EXIT_FAILURE);
		}
		else if(child == 0) {
			if((execvp(*array, array)) < 0) {
				perror("execvp");
				return 1;
			}
		}
		else {
			wait(NULL);
			execvp(argv[0], argv);
		}
	}

	return 0;
}