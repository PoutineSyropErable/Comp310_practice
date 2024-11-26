#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 31
#define ARRAY_SIZE 257
#define LOOP_NUMBER ((ARRAY_SIZE + MAX_THREADS - 1) / MAX_THREADS) // Ceiling division

typedef struct {
	int min;
	int max;
	int* a;
	int* b;
	int partial_sum;
	int partial_prod;
} SumProdArgs;

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

	// Initialize thread arguments and create threads
	for (int i = 0; i < LOOP_NUMBER; i++) {
		int block_start = i * MAX_THREADS;
		int block_end = block_start + MAX_THREADS;
		if (block_end > ARRAY_SIZE) {
			block_end = ARRAY_SIZE;
		}

		args_struct[i].a = a;
		args_struct[i].b = b;
		args_struct[i].min = block_start;
		args_struct[i].max = block_end;
		args_struct[i].partial_sum = 0;
		args_struct[i].partial_prod = 1;

		pthread_create(&threads[i], NULL, Threadcode, &args_struct[i]);
	}

	int total_sum = 0;
	int total_prod = 1;

	// Wait for threads to finish and aggregate results
	for (int i = 0; i < LOOP_NUMBER; i++) {
		pthread_join(threads[i], NULL);
		total_sum += args_struct[i].partial_sum;
		total_prod *= args_struct[i].partial_prod;
	}

	printf("Total sum: %d\n", total_sum);
	printf("Total product: %d\n", total_prod);

	return 0;
}

void* Threadcode(void* args) {
	SumProdArgs* args_struct = (SumProdArgs*)args;
	int* a = args_struct->a;
	int* b = args_struct->b;
	int partial_sum = 0;
	int partial_prod = 1;

	for (int i = args_struct->min; i < args_struct->max; i++) {
		int c = a[i] * b[i];
		partial_sum += c;
		partial_prod *= c;
	}

	args_struct->partial_sum = partial_sum;
	args_struct->partial_prod = partial_prod;

	return NULL;
}
