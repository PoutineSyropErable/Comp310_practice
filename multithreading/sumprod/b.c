
#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 32

int sum, prod;
int main() {

	int i;
	sum = 0, prod = 1;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_create();
	}
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_join();
	}

	printf("sum: %d\n", sum);
	printf("prod: %d\n", prod);
}

void Threadcode() {
	int i, c;
	for (i = my_min; i < my_max; i++) {
		c = a[i] * b[i];
		sum += c;
		prod *= c;
	}
}
