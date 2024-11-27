#include "add.h"
#include <stdio.h>
#include <stdlib.h>

void add_prog_1(char* host, int a, int b) {
	CLIENT* clnt;
	int* result;
	numbers args;

	// Create client handle
	clnt = clnt_create(host, ADD_PROG, ADD_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(EXIT_FAILURE);
	}

	// Prepare arguments
	args.a = a;
	args.b = b;

	// Call remote procedure
	result = add_1(&args, clnt);
	if (result == NULL) {
		clnt_perror(clnt, "call failed");
	} else {
		printf("Result: %d + %d = %d\n", a, b, *result);
	}

	// Clean up
	clnt_destroy(clnt);
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <server_host> <num1> <num2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char* host = argv[1];
	int num1 = atoi(argv[2]);
	int num2 = atoi(argv[3]);

	add_prog_1(host, num1, num2);
	return 0;
}
