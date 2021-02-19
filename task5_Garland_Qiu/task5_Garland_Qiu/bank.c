#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "sem.h"


#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4, *DAD_fd, *SON_1_fd, *SON_2_fd;			/* File Pointers */
int sem;
int count_1, count_2, count_3;

main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

	//Check errors
	if((sem = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1) {
		printf("sem error");
		exit(1);
	}

	//Initialize semaphore mutex
	sem_create(sem, 1);
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);

	//Initialize wait times for DAD, SON_1, SON_2
	DAD_fd = fopen("DAD", "w");
	count_1 = 0;
	fprintf(DAD_fd, "%d\n", count_1);
	fclose(DAD_fd);
	
	SON_1_fd = fopen("SON_1", "w");
	count_2 = 0;
	fprintf(SON_1_fd, "%d\n", count_2);
	fclose(SON_1_fd);

	SON_2_fd = fopen("SON_2", "w");
	count_3 = 0;
	fprintf(SON_2_fd, "%d\n", count_3);
	fclose(SON_2_fd);

	//Create child processes that will do the updates
	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=5;
		for(i=1;i<=N; i++)
		{
			//Account balance is accessed, so to prevent writing, sem is placed
			P(sem);

			SON_1_fd = fopen("SON_1", "r+");
			fscanf(SON_1_fd, "%d", &count_2);
			fseek(SON_1_fd, 0L, 0);
			count_2++;
			fprintf(SON_1_fd, "%d\n", count_2);
			fclose(SON_1_fd);

			SON_2_fd = fopen("SON_2", "r+");
			fscanf(SON_2_fd, "%d", &count_3);
			fseek(SON_2_fd, 0L, 0);
			count_3++;
			fprintf(SON_2_fd, "%d\n", count_3);
			fclose(SON_2_fd);


			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */

			//Dad finishes. Let others access the balance.
			V(sem);
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				//First son attempting to access, so need to call a semaphore.
				P(sem);

				DAD_fd = fopen("DAD", "r+");
				fscanf(DAD_fd, "%d", &count_1);
				fseek(DAD_fd, 0L, 0);
				count_1++;
				fprintf(DAD_fd, "%d\n", count_1);
				fclose(DAD_fd);

				SON_2_fd = fopen("SON_2", "r+");
				fscanf(SON_2_fd, "%d", &count_3);
				fseek(SON_2_fd, 0L, 0);
				count_3++;
				fprintf(SON_2_fd, "%d\n", count_3);
				fclose(SON_2_fd);


				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}

				//First son finished.
				V(sem);
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					//Second son attempting to access, so need to call a semaphore.
					P(sem);

					DAD_fd = fopen("DAD", "r+");
					fscanf(DAD_fd, "%d", &count_1);
					fseek(DAD_fd, 0L, 0);
					count_1++;
					fprintf(DAD_fd, "%d\n", count_1);
					fclose(DAD_fd);

					SON_1_fd = fopen("SON_1", "r+");
					fscanf(SON_1_fd, "%d", &count_2);
					fseek(SON_1_fd, 0L, 0);
					count_2++;
					fprintf(SON_1_fd, "%d\n", count_2);
					fclose(SON_1_fd);


					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}

					//Second son finished
					V(sem);
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}