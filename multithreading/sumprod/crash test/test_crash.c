#include <stdlib.h>

int main() {
    int *p = NULL;   // Create a NULL pointer
    *p = 42;         // Dereference the NULL pointer (causes segmentation fault)
    return 0;
}
