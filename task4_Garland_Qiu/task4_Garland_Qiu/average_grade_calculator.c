#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int chapters, homeworks, row_count = 0, column_count = 0, managers = 0, workers = 0, status;
	double average = 0;

	printf("Enter the number of chapters: ");
	scanf("%d", &chapters);
	printf("Enter the number of homeworks per chapter: ");
	scanf("%d", &homeworks);
	const int rows = 10, columns = chapters * homeworks;

	double grades[columns][rows];
	double averages[columns];

	FILE *source = fopen("quiz_grades.txt", "r");

	while(row_count < rows) {
		while(column_count < columns) {
			fscanf(source, "%lf", &grades[column_count][row_count]);
			column_count++;
		}
		column_count = 0;
		row_count++;
	}

	row_count = 0;
	column_count = 0;

	while(managers < chapters) {
		wait(&status);
		pid_t manager_child = fork();

		if(manager_child < 0) {
			fprintf(stderr, "Manager Process Failed to Fork\n");
			exit(EXIT_FAILURE);
		}
		else if(manager_child == 0) {
			column_count = homeworks * managers;

			while(workers < homeworks) {
				wait(&status);
				pid_t worker_child = fork();

				if(worker_child < 0) {
					fprintf(stderr, "Worker Child Failed to Fork\n");
					exit(EXIT_FAILURE);
				}
				else if(worker_child == 0) {
					while(row_count < rows) {
						average += grades[column_count][row_count];
						row_count++;
					}
					averages[column_count] = average / rows;
					printf("Average of Homework %d: %f \n",(column_count + 1), averages[column_count]);
					exit(0);
				}
				else {
					wait(NULL);
				}
				row_count = 0;
				column_count++;
				workers++;
			}
			exit(0);
		}
		else {
			wait(NULL);
		}

		workers = 0;
		managers++;
	}

	fclose(source);
	return 0;
}