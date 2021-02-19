#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 10000
#define REQUESTS 1000

int random_array[REQUESTS];
int start = 0;

// FCFS: First Come First Serve: Requests are addressed in the order they arrive in the disk queue.
int FCFS(int *random_array) {
	int i, head = 0;
	int FCFS_start = random_array[start];
	for(i = start; i < REQUESTS; i++) {
		head += abs(random_array[i] - FCFS_start);
	}
	for(i = 0; i < start; i++) {
		head += abs(FCFS_start - random_array[i]);
	}
	return head;
}

// Sorting the array for SSTF
int* sort_array() {
	int temp, i, j = 0;
	for(i = 0; i < REQUESTS; ++i) {
		for(j = i + 1; j < REQUESTS; ++j) {
			if(random_array[i] > random_array[j]) {
				temp = random_array[i];
				random_array[i] = random_array[j];
				random_array[j] = temp;
			}
		}
	}
	return random_array;
}

// SSTF: Shortest Seek Time First: Requests having shotest seek time are executed first.
int SSTF(int *random_array) {
	random_array = sort_array();
	int small_i = start - 1;
	int large_i = start + 1;
	int small_diff, large_diff, head = 0;
	int total = REQUESTS - 2;
	int new_head = start;
	int head_value = random_array[start];

	while(total >= 0) {
		small_diff = abs(random_array[new_head] - random_array[small_i]);
		large_diff = abs(random_array[large_i] - random_array[new_head]);

		if(small_diff < large_diff) {
			head += small_diff;
			new_head = small_i;
			small_i--;
		}
		else {
			head += large_diff;
			new_head = large_i;
			large_i++;
		}

		total--;
	}

	return head;
}

// SCAN: The disk arm moves in a direction and services the request coming in its path and after
// reaching the end of the disk, it reverses the direction and services the request arriving in the path.
int SCAN(int *random_array) {
	int i, current_value, difference, head, current_i = 0;
	int s_value = random_array[start];

	for(i = start - 1; i >= 0; --i) {
		current_value = random_array[i];
		difference = abs(s_value - current_value);
		head += difference;
		s_value = current_value;
	}

	head += s_value;
	s_value = 0;

	for(i = start + 1; i < REQUESTS; i++) {
		current_value = random_array[i];
		difference = abs(current_value - s_value);
		head += difference;
		s_value = current_value;
	}

	return head;
}

// CSCAN: The disk arm scans the path that has been scanned after reversing its direction.
int CSCAN(int *random_array) {
	int i, current_value, difference, head, current_i = 0;
	int upper_bound = 9999;
	int s_value = random_array[start];

	for(i = start + 1; i < REQUESTS; i++) {
		current_value = random_array[i];
		difference = abs(s_value - current_value);
		head += difference;
		s_value = current_value;
	}

	head += upper_bound - s_value;
	s_value = 0;
	head += 9999;

	for(i = 0; i < start; i++) {
		current_value = random_array[i];
		difference = abs(current_value - s_value);
		head += difference;
		s_value = current_value;
	}

	return head;
}

// LOOK: Similiar to SCAN except for the difference that the disk arm in spite of going to the end
// of the disk and start servicing request from there.
int LOOK(int *random_array) {
	int i, current_value, difference, head, current_i = 0;
	int s_value = random_array[start];

	for(i = start + 1; i < REQUESTS; i++) {
		current_value = random_array[i];
		difference = abs(s_value - current_value);
		head += difference;
		s_value = current_value;
	}

	for(i = start - 1; i >= 0; --i) {
		current_value = random_array[i];
		difference = abs(current_value - s_value);
		head += difference;
		s_value = current_value;
	}

	return head;
}


int main(int argc, char*argv[]) {
	if(argc != 2) {
		printf("Too few or too many arguments. Need to give one starting index number from 0-9999.\n");
		return 1;
	}

	// Just taking the number as the index in cmd when running
	start = atoi(argv[1]);

	// Setting random cylinders
	for(int i = 0; i < REQUESTS; i++) {
		random_array[i] = rand() % 10000;
	}

	printf("Starting Index: %d\n", start);
	printf("Starting Value: %d\n\n", random_array[start]);

	printf("FCFS amount of head movements: %d\n", FCFS(random_array));
	printf("SSTF amount of head movements: %d\n", SSTF(random_array));
	printf("SCAN amount of head movements: %d\n", SCAN(random_array));
	printf("C-SCAN amount of head movements: %d\n", CSCAN(random_array));
	printf("LOOK amount of head movements: %d\n", LOOK(random_array));

	return 0;
}