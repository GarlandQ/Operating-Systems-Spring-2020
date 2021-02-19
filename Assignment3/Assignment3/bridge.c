#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// Counters for the number of farmers
int north_counter, south_counter = 0;
// Initialize pthread mutex. We can comment out this line later to see outputs without locks.
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// North Tunbridge farmer
void *north(void *vargp) {
	// Locking. We can comment out this line later to see outputs without locks.
	pthread_mutex_lock(&lock);
	north_counter++;
	printf("North Tunbridge #%d farmer can cross the bridge\n", north_counter);
	printf("North Tunbridge #%d is traveling on the bridge...\n", north_counter);
	// Sleeping
	sleep(3);
	printf("North Tunbridge #%d farmer has left the bridge\n\n", north_counter);
	// Releasing the lock. We can comment out this line later to see outputs without locks.
	pthread_mutex_unlock(&lock);
}

// South Tunbridge farmer
void *south(void *vargp) {
	// Locking. We can comment out this line later to see outputs without locks.
	pthread_mutex_lock(&lock);
	south_counter++;
	printf("South Tunbridge #%d farmer can cross the bridge\n", south_counter);
	printf("South Tunbridge #%d is traveling on the bridge...\n", south_counter);
	// Sleeping
	sleep(3);
	printf("South Tunbridge #%d farmer has left the bridge\n\n", south_counter);
	// Releasing the lock. We can comment out this line later to see outputs without locks.
	pthread_mutex_unlock(&lock);
}

int main() {
	// Declare two pthreads: North and South
	pthread_t tid[2];

	int i = 0;
	// Can change value in while loop to see more farmers cross the bridge if needed.
	while(i < 5) {
		// Creating the pthreads for north and south
		pthread_create(&tid[0], NULL, &north, NULL);
		pthread_create(&tid[1], NULL, &south, NULL);
		i++;
	}

	// Suspend execution of threads until the thread terminates.
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	// Destroy lock. We can comment out this line later to see outputs without locks.
	pthread_mutex_destroy(&lock);
	return 0;
}