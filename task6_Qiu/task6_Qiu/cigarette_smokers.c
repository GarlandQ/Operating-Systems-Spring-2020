// Garland Qiu
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobacco = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match = PTHREAD_MUTEX_INITIALIZER;

void *supply(void *vargp) {
	int random_number = 0;
	int a = 0;
	do {
		pthread_mutex_lock(&lock);
		random_number = rand() % 3;
		printf("RANDOM NUMBER = %d\n", random_number);
		if(random_number == 0) {
			printf("Agent puts TOBACCO and PAPER on the table.\n");
			pthread_mutex_unlock(&match);
		}
		else if(random_number == 1) {
			printf("Agent puts TOBACCO and MATCH on the table.\n");
			pthread_mutex_unlock(&paper);
		}
		else {
			printf("Agent puts PAPER and MATCH on the table.\n");
			pthread_mutex_unlock(&tobacco);
		}
		pthread_mutex_unlock(&lock);
		pthread_mutex_lock(&agent);
		a++;
	} while(a < 10);
}

void *smoker_1(void *vargp) {
	int a = 0;
	do {
		pthread_mutex_lock(&tobacco);
		pthread_mutex_lock(&lock);
		printf("First smoker picks up MATCH and PAPER.\n");
		printf("First smoker is smoking cigarettes.\n");
		sleep(1);
		pthread_mutex_unlock(&agent);
		pthread_mutex_unlock(&lock);
		a++;
	} while(a < 10);
}

void *smoker_2(void *vargp) {
	int a = 0;
	do {
		pthread_mutex_lock(&paper);
		pthread_mutex_lock(&lock);
		printf("Second smoker picks up TOBACCO and MATCH.\n");
		printf("Second smoker is smoking cigarettes.\n");
		sleep(1);
		pthread_mutex_unlock(&agent);
		pthread_mutex_unlock(&lock);
		a++;
	} while(a < 10);
}

void *smoker_3(void *vargp) {
	int a = 0;
	do {
		pthread_mutex_lock(&match);
		pthread_mutex_lock(&lock);
		printf("Third smoker picks up TOBACCO and PAPER.\n");
		printf("Third smoker is smoking cigarettes.\n");
		sleep(1);
		pthread_mutex_unlock(&agent);
		pthread_mutex_unlock(&lock);
		a++;
	} while(a < 10);
}

int main() {
	pthread_mutex_init(&agent, NULL);
	pthread_mutex_lock(&agent);
	pthread_mutex_init(&tobacco, NULL);
	pthread_mutex_lock(&tobacco);
	pthread_mutex_init(&match, NULL);
	pthread_mutex_lock(&match);
	pthread_mutex_init(&paper, NULL);
	pthread_mutex_lock(&paper);
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_unlock(&lock);

	pthread_t tid[4];
	pthread_create(&tid[0], NULL, &supply, NULL);
	pthread_create(&tid[1], NULL, &smoker_1, NULL);
	pthread_create(&tid[2], NULL, &smoker_2, NULL);
	pthread_create(&tid[3], NULL, &smoker_3, NULL);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);

	pthread_mutex_destroy(&lock);

	return 0;
}