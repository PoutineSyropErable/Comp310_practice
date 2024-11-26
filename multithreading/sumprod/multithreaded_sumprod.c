#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 31
#define ARRAY_SIZE 257
#define LOOP_NUMBER (ARRAY_SIZE + MAX_THREADS - 1) / MAX_THREADS // Ceiling division

typedef struct {
	int min;
	int* a;
	int* b;
} SumProdArgs;

pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prod_lock = PTHREAD_MUTEX_INITIALIZER;

void* Threadcode(void* args);

int sum = 0, prod = 1;

int main() {
	pthread_t threads[MAX_THREADS];

	int a[ARRAY_SIZE];
	int b[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		a[i] = i + 1;
		b[i] = 2;
	}

	SumProdArgs args_struct[LOOP_NUMBER];

	for (int i = 0, block_start = 0; block_start < ARRAY_SIZE; i++, block_start += MAX_THREADS) {
		printf(" i:%d , block_start: %d \n", i, block_start);

		args_struct[i].a = a;
		args_struct[i].b = b;
		args_struct[i].min = block_start;
		pthread_create(&threads[i], NULL, Threadcode, &args_struct[i]);
	}
	for (int i = 0; i < LOOP_NUMBER; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("sum: %d\n", sum);
	printf("prod: %d\n", prod);

	return 0;
}

void* Threadcode(void* args) {

	SumProdArgs* args_struct = (SumProdArgs*)args;
	int my_min = args_struct->min;
	int my_max = my_min + MAX_THREADS;
	if (my_max > ARRAY_SIZE) {
		my_max = ARRAY_SIZE;
	}
	int* a = args_struct->a;
	int* b = args_struct->b;

	for (int i = my_min; i < my_max; i++) {
		int c = a[i] * b[i];
		// ^^ This line above isn't modifying a shared variable. c is saved on stack, and each threads have a different stack
		// Hence it doesn't need a lock
		pthread_mutex_lock(&sum_lock);
		sum += c;
		pthread_mutex_unlock(&sum_lock);

		pthread_mutex_lock(&prod_lock);
		prod *= c;
		pthread_mutex_unlock(&prod_lock);
	}

	return NULL;
}
