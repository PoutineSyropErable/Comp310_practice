#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 31
#define ARRAY_SIZE 257
#define LOOP_NUMBER ((ARRAY_SIZE + MAX_THREADS - 1) / MAX_THREADS) // Ceiling division

typedef struct {
	int min;
	int max;
	int* a;
	int* b;
} SumProdArgs;

typedef struct {
	int partial_sum;
	int partial_prod;
} SumProdResult;

void* Threadcode(void* args);

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
	for (int i = 0, block_start = 0; i < LOOP_NUMBER; i++, block_start += MAX_THREADS) {
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

	int total_sum = 0;
	int total_prod = 1;

	// Collect results from threads
	for (int i = 0; i < LOOP_NUMBER; i++) {
		SumProdResult* result;
		pthread_join(threads[i], (void**)&result);

		total_sum += result->partial_sum;
		total_prod *= result->partial_prod;

		// Free the allocated memory
		free(result);
	}

	printf("Total Sum: %d\n", total_sum);
	printf("Total Product: %d\n", total_prod);

	return 0;
}

void* Threadcode(void* args) {
	SumProdArgs* args_struct = (SumProdArgs*)args;
	int* a = args_struct->a;
	int* b = args_struct->b;

	// Allocate memory for the result
	SumProdResult* result = malloc(sizeof(SumProdResult));
	if (result == NULL) {
		perror("Failed to allocate memory");
		pthread_exit(NULL);
	}

	result->partial_sum = 0;
	result->partial_prod = 1;

	// Compute partial sum and product
	for (int i = args_struct->min; i < args_struct->max; i++) {
		int c = a[i] * b[i];
		result->partial_sum += c;
		result->partial_prod *= c;
	}

	// Return the result via pthread_exit
	pthread_exit((void*)result);
}
