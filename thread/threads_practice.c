#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 5

typedef struct {
  float *array;
  int block_number;
} ParallelArrayProcessingArgument;

// Function for the threads to execute
void *parallel_double_array(void *arg) {
  ParallelArrayProcessingArgument *data =
      (ParallelArrayProcessingArgument *)arg;
  int start_index = data->block_number * BLOCK_SIZE;
  for (int i = start_index; i < start_index + BLOCK_SIZE; i++) {
    data->array[i] *= 2; // Perform the write operation
  }
  pthread_exit(NULL);
}

int main() {
  printf("\n\n-------------------\n\n");
  int n = 4; // Number of blocks
  int array_size = BLOCK_SIZE * n;
  float *array = malloc(array_size * sizeof(float));

  // Initialize the array with a[i] = i
  for (int i = 0; i < array_size; i++) {
    array[i] = i;
  }

  pthread_t threads[n];
  ParallelArrayProcessingArgument thread_data[n];

  // Create threads
  for (int i = 0; i < n; i++) {
    thread_data[i].array = array;
    thread_data[i].block_number = i;
    if (pthread_create(&threads[i], NULL, parallel_double_array,
                       &thread_data[i]) != 0) {
      perror("Failed to create thread");
      free(array);
      return 1;
    }
  }

  // Wait for all threads to finish
  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], NULL);
  }

  // Print the resulting array
  for (int i = 0; i < array_size; i++) {
    printf("%.1f ", array[i]);
  }
  printf("\n");

  free(array);
  return 0;
}
