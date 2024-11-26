#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// I have a 7-3xd So 16 logical processes. Multiply by two, and make it a near prime for bounds testing
#define MAX_THREADS 31
#define ARRAY_SIZE 257
#define LOOP_NUMBER ((ARRAY_SIZE + MAX_THREADS - 1) / MAX_THREADS) // Ceiling division

int sum = 0, prod = 1; // Global variables
pthread_mutex_t sumlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prodlock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int min;
	int max;
	int* a;
	int* b;
} SumProdArgs;

int get_max(int block_start) {

	int block_end = block_start + MAX_THREADS;
	if (block_end > ARRAY_SIZE) {
		block_end = ARRAY_SIZE;
	}
	return block_end;

	// If you want to keep to the notes and not have a max, then you could all this function
	// Inside Threadcode and just use that, rather then have it calculated in main
	// It would technically be faster then this code, as its more code inside a thread
}

void* Threadcode(void* args) {
	SumProdArgs* args_struct = (SumProdArgs*)args;
	int* a = args_struct->a;
	int* b = args_struct->b;

	int local_sum = 0;  // Local sum for this thread
	int local_prod = 1; // Local product for this thread

	// Compute local sum and product
	for (int i = args_struct->min; i < args_struct->max; i++) {
		int c = a[i] * b[i];
		local_sum += c;
		local_prod *= c;
	}

	// Lock and update global sum
	pthread_mutex_lock(&sumlock);
	sum += local_sum;
	pthread_mutex_unlock(&sumlock);

	// Lock and update global product
	pthread_mutex_lock(&prodlock);
	prod *= local_prod;
	pthread_mutex_unlock(&prodlock);

	return NULL;
}

int main() {
	printf("\n\n-------------------START OF MAIN CODE-------------------\n\n");
	pthread_t threads[LOOP_NUMBER];
	SumProdArgs args_struct[LOOP_NUMBER];

	int a[ARRAY_SIZE];
	int b[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		a[i] = i + 1;
		b[i] = 2;
	}

	// Divide work among threads
	for (int i = 0, block_start = 0; block_start < ARRAY_SIZE; i++, block_start += MAX_THREADS) {
		int block_end = block_start + MAX_THREADS;
		if (block_end > ARRAY_SIZE) {
			block_end = ARRAY_SIZE;
		}

		args_struct[i].a = a;
		args_struct[i].b = b;
		args_struct[i].min = block_start;
		args_struct[i].max = block_end;

		pthread_create(&threads[i], NULL, Threadcode, &args_struct[i]);
	}

	// Wait for all threads to finish
	for (int i = 0; i < LOOP_NUMBER; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Global Sum: %d\n", sum);
	printf("Global Product: %d\n", prod);

	return 0;
}
