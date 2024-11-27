#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sigjmp_buf env;

// Signal handler to catch segmentation faults
void handle_sigsegv(int sig) {
  siglongjmp(env, 1); // Jump back to the point of failure
}

// Function to test reading/writing bytes **after** the allocated memory
void test_after(uint8_t *ptr) {
  size_t counter = 0;

  printf("Testing bytes AFTER the allocated memory:\n");

  for (size_t offset = 0; offset < 1024 * 1024;
       offset++) { // Test up to 1 MiB after
    if (sigsetjmp(env, 1) == 0) {
      ptr[offset] = 0xAA;                  // Attempt to write
      volatile uint8_t temp = ptr[offset]; // Attempt to read
      (void)temp;                          // Prevent compiler optimizations
      counter++;
    } else {
      // Segmentation fault occurred
      printf("Segmentation fault at offset +%zu bytes\n", offset);
      break;
    }
  }

  printf("Successfully accessed %zu bytes after the allocated memory.\n",
         counter);
}

// Function to test reading/writing bytes **before** the allocated memory
void test_before(uint8_t *ptr) {
  size_t counter = 0;

  printf("Testing bytes BEFORE the allocated memory:\n");

  for (size_t offset = 1; offset < 1024 * 1024;
       offset++) { // Test up to 1 MiB before
    if (sigsetjmp(env, 1) == 0) {
      ptr[-(ssize_t)offset] = 0xAA;                  // Attempt to write
      volatile uint8_t temp = ptr[-(ssize_t)offset]; // Attempt to read
      (void)temp; // Prevent compiler optimizations
      counter++;
    } else {
      // Segmentation fault occurred
      printf("Segmentation fault at offset -%zu bytes\n", offset);
      break;
    }
  }

  printf("Successfully accessed %zu bytes before the allocated memory.\n",
         counter);
}

int main() {
  // Set up the signal handler for segmentation faults
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigsegv;
  sigaction(SIGSEGV, &sa, NULL);

  // Allocate 1 byte of memory
  uint8_t *allocated = malloc(1);
  if (!allocated) {
    perror("Failed to allocate memory");
    return EXIT_FAILURE;
  }

  printf("Allocated memory at: %p\n", (void *)allocated);

  // Test bytes after and before the allocated memory
  test_after(allocated);
  test_before(allocated);

  // Free the allocated memory
  free(allocated);

  return EXIT_SUCCESS;
}
