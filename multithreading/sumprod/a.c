#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 32
#define ARRAY_SIZE 256

int sum = 0, prod = 1;

pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prod_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int min;
	int* a;
	int* b;
} SumProdArgs;

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
		pthread_mutex_lock(&sum_lock);
		sum += c;
		pthread_mutex_unlock(&sum_lock);

		pthread_mutex_lock(&prod_lock);
		prod *= c;
		pthread_mutex_unlock(&prod_lock);
	}

	return NULL;
}

int main() {
	pthread_t threads[MAX_THREADS];

	int a[ARRAY_SIZE];
	int b[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		a[i] = i;
		b[i] = i / 2 + 1;
	}

	SumProdArgs args_struct = {.min = 0, .a = a, .b = b};

	for (int i = 0, block_start = 0; block_start < ARRAY_SIZE; i++, block_start += MAX_THREADS) {
		printf(" i:%d , block_start: %d \n", i, block_start);

		args_struct.min = block_start;
		pthread_create(&threads[i], NULL, Threadcode, &args_struct);
	}
	for (int i = 0; i < MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("%d\n", sum);
	printf("%d\n", prod);

	return 0;
}
